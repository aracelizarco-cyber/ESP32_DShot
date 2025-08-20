/*
 * Standalone ESP-IDF greenhouse controller
 * - Wi‑Fi STA using provided SSID/PASS
 * - MQTT client to HA broker, with discovery for configurable fans
 * - DShot600 control using RMT encoder from dshot_esc_encoder.c/h (GPIOs set in menuconfig)
 * - MQTT/HA discovery and smooth ramping; telemetry removed for now
 */

#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <inttypes.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
//

#include "esp_log.h"
#include "esp_event.h"
#include "esp_timer.h"
#include "esp_wifi.h"
#include "esp_netif.h"
#include "esp_netif_ip_addr.h"
#include "nvs_flash.h"
#include "mqtt_client.h"
#include "driver/gpio.h"
#include "driver/rmt_tx.h"
// No UART/telemetry used

#include "dshot_esc_encoder.h"

#include "sdkconfig.h"
#ifndef CONFIG_WIFI_SSID
#define CONFIG_WIFI_SSID "Tent"
#endif
#ifndef CONFIG_WIFI_PASS
#define CONFIG_WIFI_PASS "1LoveW33D"
#endif
#ifndef CONFIG_MQTT_BROKER_URI
#define CONFIG_MQTT_BROKER_URI "mqtt://192.168.53.174"
#endif
#ifndef CONFIG_MQTT_USERNAME
#define CONFIG_MQTT_USERNAME   "greenhouse_esp"
#endif
#ifndef CONFIG_MQTT_PASSWORD
#define CONFIG_MQTT_PASSWORD   "1LoveW33D"
#endif


// Modular configuration via Kconfig (see Kconfig file)
#ifndef CONFIG_FAN_COUNT
#define CONFIG_FAN_COUNT 2
#endif
#ifndef CONFIG_FAN_INDEX_START
#define CONFIG_FAN_INDEX_START 1
#endif
#ifndef CONFIG_DSHOT_GPIO
#define CONFIG_DSHOT_GPIO "5,7"
#endif
#ifndef CONFIG_MIN_SPIN_PCT
#define CONFIG_MIN_SPIN_PCT 15
#endif
#ifndef CONFIG_RAMP_STEP_PCT
#define CONFIG_RAMP_STEP_PCT 3
#endif
#ifndef CONFIG_RAMP_TICK_MS
#define CONFIG_RAMP_TICK_MS 20
#endif
// Parse DSHOT GPIOs from config string
#include <stdlib.h>
static int dshot_gpios[8] = {0};
static int fan_count = CONFIG_FAN_COUNT;
static int fan_index_start = CONFIG_FAN_INDEX_START;
static int min_spin_pct = CONFIG_MIN_SPIN_PCT;
static int ramp_step_pct = CONFIG_RAMP_STEP_PCT;
static int ramp_tick_ms = CONFIG_RAMP_TICK_MS;

static void parse_dshot_gpios(void) {
    const char *str = CONFIG_DSHOT_GPIO;
    char *copy = strdup(str);
    char *token = strtok(copy, ",");
    int idx = 0;
    while (token && idx < 8) {
        dshot_gpios[idx++] = atoi(token);
        token = strtok(NULL, ",");
    }
    free(copy);
}

#if CONFIG_IDF_TARGET_ESP32H2
#define DSHOT_ESC_RESOLUTION_HZ 32000000
#else
#define DSHOT_ESC_RESOLUTION_HZ 40000000
#endif

static const char *TAG = "greenhouse";
static EventGroupHandle_t s_wifi_event_group;
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_GOT_IP_BIT    BIT1
static esp_netif_t *s_wifi_netif = NULL;

// Forward declaration
static void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);

typedef struct {
    rmt_channel_handle_t chan;
    rmt_encoder_handle_t enc600;
    rmt_encoder_handle_t active_enc;
    dshot_esc_throttle_t throttle;
    int gpio;
    bool active;
    // Ramping/control state
    uint8_t current_pct;   // last applied percentage (0..100)
    uint8_t target_pct;    // desired percentage (0..100)
    uint32_t last_dshot;   // last applied raw DShot value + telemetry bit (composite)
} motor_ctx_t;

static motor_ctx_t motors[8] = {0};
static esp_mqtt_client_handle_t mqtt = NULL;
// Fixed protocol: DSHOT600 only
static volatile bool s_mqtt_connected = false;

// (Telemetry removed)

// Helper: map 0-100% to DShot throttle range (48..2047)
static uint16_t pct_to_dshot(uint8_t pct)
{
    // Important: Never send value 0 in continuous stream; 0..47 are special commands in DShot.
    // Use 48 for 0% (idle) and 2047 for 100%.
    if (pct == 0) return 48;
    if (pct > 0 && pct < min_spin_pct) pct = min_spin_pct;
    const uint16_t min = 48;
    const uint16_t max = 2047;
    uint32_t val = min + (uint32_t)(pct) * (max - min) / 100U;
    if (val > max) val = max;
    return (uint16_t)val;
}

// (Telemetry CRC removed)

static void motor_apply(motor_ctx_t *m)
{
    if (!m || !m->chan || (!m->enc600)) return;
    // Coerce illegal 0..47 to 48 to prevent sending DShot special commands continuously
    if (m->throttle.throttle < 48) {
        m->throttle.throttle = 48;
        m->throttle.telemetry_req = false;
    }
    uint32_t composite = ((uint32_t)m->throttle.throttle << 1) | (m->throttle.telemetry_req ? 1u : 0u);
    if (composite == m->last_dshot) {
        return;
    }
    rmt_transmit_config_t tx_cfg = {
        .loop_count = -1, // repeat until we re-enable
    };
    if (m->active) {
        // Cleanly stop the previous infinite loop before updating
        ESP_ERROR_CHECK(rmt_disable(m->chan));
        // After disabling, ensure TX engine considers previous transaction completed
        ESP_ERROR_CHECK(rmt_tx_wait_all_done(m->chan, portMAX_DELAY));
        // Reset the encoder state machine to start fresh
        if (m->active_enc) {
            rmt_encoder_reset(m->active_enc);
        }
    }
    rmt_encoder_handle_t enc = m->active_enc ? m->active_enc : m->enc600;
    ESP_ERROR_CHECK(rmt_enable(m->chan));
    ESP_ERROR_CHECK(rmt_transmit(m->chan, enc, &m->throttle, sizeof(m->throttle), &tx_cfg));
    m->active = true;
    m->last_dshot = composite;
}

// (Locked apply removed)

static void ha_publish_discovery(void)
{
    // Availability topic per node
    char avail_topic[64];
    uint8_t mac[6] = {0};
    esp_wifi_get_mac(WIFI_IF_STA, mac);
    char mac_suffix[7];
    snprintf(mac_suffix, sizeof(mac_suffix), "%02X%02X%02X", mac[3], mac[4], mac[5]);
    snprintf(avail_topic, sizeof(avail_topic), "greenhouse_esp/status/%s", mac_suffix);

    for (int i = 0; i < fan_count; ++i) {
        int fan_idx = fan_index_start + i;
        char disc[128];
    char uniq[64];
        snprintf(disc, sizeof(disc), "homeassistant/fan/greenhouse_esp_motor%d_%s/config", fan_idx, mac_suffix);
        snprintf(uniq, sizeof(uniq), "greenhouse_esp_motor%d_%s", fan_idx, mac_suffix);

        char payload[640];
        
        snprintf(payload, sizeof(payload),
                 "{"
                 "\"name\":\"Tent Fan %d\"," 
                 "\"unique_id\":\"%s\"," 
                 "\"command_topic\":\"greenhouse_esp/fan%d/set\"," 
                 "\"state_topic\":\"greenhouse_esp/fan%d/state\"," 
                 "\"percentage_command_topic\":\"greenhouse_esp/fan%d/percentage/set\"," 
                 "\"percentage_state_topic\":\"greenhouse_esp/fan%d/percentage\"," 
                 "\"payload_on\":\"ON\",\"payload_off\":\"OFF\"," 
                 "\"availability_topic\":\"%s\","
                 "\"payload_available\":\"online\",\"payload_not_available\":\"offline\""
                 "}",
                 fan_idx, uniq, fan_idx, fan_idx, fan_idx, fan_idx, avail_topic);
        esp_mqtt_client_publish(mqtt, disc, payload, 0, 1, true);
        
    }
    // Set node online
    esp_mqtt_client_publish(mqtt, avail_topic, "online", 0, 1, true);
}

static void publish_states(void)
{
    if (!mqtt || !s_mqtt_connected) return;
    char buf[16];
    for (int i = 0; i < fan_count; ++i) {
        int fan_idx = fan_index_start + i;
        uint8_t pct = motors[i].current_pct;
        const char *state = (pct > 0) ? "ON" : "OFF";
        char topic_state[64];
        char topic_pct[64];
        snprintf(topic_state, sizeof(topic_state), "greenhouse_esp/fan%d/state", fan_idx);
        snprintf(topic_pct, sizeof(topic_pct), "greenhouse_esp/fan%d/percentage", fan_idx);
        esp_mqtt_client_publish(mqtt, topic_state, state, 0, 1, true);
        snprintf(buf, sizeof(buf), "%u", pct);
        esp_mqtt_client_publish(mqtt, topic_pct, buf, 0, 1, true);
    }
}

static void mqtt_on_message(const char *topic, const char *data, int len)
{
    char dbg_payload[64] = {0};
    int dbg_len = len < (int)sizeof(dbg_payload)-1 ? len : (int)sizeof(dbg_payload)-1;
    memcpy(dbg_payload, data, dbg_len);
    ESP_LOGI(TAG, "MQTT RX topic='%s' payload='%s'", topic, dbg_payload);

    for (int i = 0; i < fan_count; ++i) {
        int fan_idx = fan_index_start + i;
        char set_topic[64];
        char pct_set_topic[64];
        snprintf(set_topic, sizeof(set_topic), "greenhouse_esp/fan%d/set", fan_idx);
        snprintf(pct_set_topic, sizeof(pct_set_topic), "greenhouse_esp/fan%d/percentage/set", fan_idx);

        if (strcmp(topic, set_topic) == 0) {
            if (len >= 2 && strncasecmp(data, "ON", 2) == 0) {
                motors[i].target_pct = min_spin_pct; // default ON
            } else if (len >= 3 && strncasecmp(data, "OFF", 3) == 0) {
                motors[i].target_pct = 0; // idle
            } else {
                ESP_LOGW(TAG, "fan%d/set unknown payload '%s' ignored", fan_idx, dbg_payload);
                return;
            }
            publish_states();
            return;
        }
        if (strcmp(topic, pct_set_topic) == 0) {
            int pct = atoi(data);
            if (pct < 0) pct = 0;
            if (pct > 100) pct = 100;
            motors[i].target_pct = (uint8_t)pct;
            ESP_LOGI(TAG, "fan%d target -> pct=%d", fan_idx, pct);
            publish_states();
            return;
        }
    }
}

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    esp_mqtt_event_handle_t event = (esp_mqtt_event_handle_t)event_data;
    switch (event_id) {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "MQTT connected");
        s_mqtt_connected = true;
        // Subscribe to control topics per fan
        for (int i = 0; i < fan_count; ++i) {
            int fan_idx = fan_index_start + i;
            char set_topic[64];
            char pct_set_topic[64];
            snprintf(set_topic, sizeof(set_topic), "greenhouse_esp/fan%d/set", fan_idx);
            snprintf(pct_set_topic, sizeof(pct_set_topic), "greenhouse_esp/fan%d/percentage/set", fan_idx);
            esp_mqtt_client_subscribe(event->client, set_topic, 1);
            esp_mqtt_client_subscribe(event->client, pct_set_topic, 1);
        }
        // Publish discovery
        mqtt = event->client;
        ha_publish_discovery();
        publish_states();
        break;
    case MQTT_EVENT_DISCONNECTED:
        s_mqtt_connected = false;
        break;
    case MQTT_EVENT_DATA: {
        char topic[256];
        char *tgt = topic;
        int tlen = event->topic_len;
        if (tlen >= (int)sizeof(topic)) tlen = sizeof(topic) - 1;
        memcpy(tgt, event->topic, tlen);
        tgt[tlen] = 0;
        // data is not null-terminated
        char data[64];
        int dlen = event->data_len;
        if (dlen >= (int)sizeof(data)) dlen = sizeof(data) - 1;
        memcpy(data, event->data, dlen);
        data[dlen] = 0;
        mqtt_on_message(topic, data, dlen);
        break; }
    default:
        if (event_id == MQTT_EVENT_ERROR) {
            ESP_LOGE(TAG, "MQTT error: link/down or auth failure");
        }
        break;
    }
}

static void wifi_init_sta(void)
{
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    s_wifi_netif = esp_netif_create_default_wifi_sta();

    s_wifi_event_group = xEventGroupCreate();

    // Register Wi‑Fi and IP event handlers
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID,
                                                        &wifi_event_handler, NULL, NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP,
                                                        &wifi_event_handler, NULL, NULL));

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    wifi_config_t wifi_config = { 0 };
    strlcpy((char *)wifi_config.sta.ssid, CONFIG_WIFI_SSID, sizeof(wifi_config.sta.ssid));
    strlcpy((char *)wifi_config.sta.password, CONFIG_WIFI_PASS, sizeof(wifi_config.sta.password));
    wifi_config.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;
    wifi_config.sta.sae_pwe_h2e = WPA3_SAE_PWE_UNSPECIFIED;

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    // Improve latency/reliability of TCP/MQTT by disabling Wi‑Fi power save
    // (can otherwise cause sporadic connect/select timeouts on some APs)
    ESP_ERROR_CHECK(esp_wifi_set_ps(WIFI_PS_NONE));

    ESP_LOGI(TAG, "wifi_init_sta finished, waiting for IP...");
}

static void mqtt_start(void)
{
    // Build unique client_id from MAC
    uint8_t mac[6] = {0};
    esp_wifi_get_mac(WIFI_IF_STA, mac);
    static char client_id[32];
    snprintf(client_id, sizeof(client_id), "greenhouse_esp_%02X%02X%02X", mac[3], mac[4], mac[5]);

    char avail_topic[64];
    snprintf(avail_topic, sizeof(avail_topic), "greenhouse_esp/status/%02X%02X%02X", mac[3], mac[4], mac[5]);
    esp_mqtt_client_config_t cfg = (esp_mqtt_client_config_t) {
    .broker.address.uri = CONFIG_MQTT_BROKER_URI,
        .broker.address.port = 1883,
    .credentials.username = CONFIG_MQTT_USERNAME,
    .credentials.authentication.password = CONFIG_MQTT_PASSWORD,
        .credentials.client_id = client_id,
        .session.last_will = {
            .topic = avail_topic,
            .msg = "offline",
            .msg_len = 7,
            .qos = 1,
            .retain = true,
        },
    };
    mqtt = esp_mqtt_client_init(&cfg);
    esp_mqtt_client_register_event(mqtt, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(mqtt);
}

static void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT) {
        switch (event_id) {
        case WIFI_EVENT_STA_START:
            ESP_LOGI(TAG, "Wi‑Fi start, connecting...\n");
            esp_wifi_connect();
            break;
        case WIFI_EVENT_STA_CONNECTED:
            ESP_LOGI(TAG, "Wi‑Fi connected to AP");
            xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
            break;
        case WIFI_EVENT_STA_DISCONNECTED:
            ESP_LOGW(TAG, "Wi‑Fi disconnected, retrying...");
            esp_wifi_connect();
            xEventGroupClearBits(s_wifi_event_group, WIFI_GOT_IP_BIT);
            break;
        default:
            break;
        }
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t *e = (ip_event_got_ip_t *)event_data;
        ESP_LOGI(TAG, "Got IP: " IPSTR ", GW: " IPSTR ", Netmask: " IPSTR,
                 IP2STR(&e->ip_info.ip), IP2STR(&e->ip_info.gw), IP2STR(&e->ip_info.netmask));
        xEventGroupSetBits(s_wifi_event_group, WIFI_GOT_IP_BIT);
    }
}

static void dshot_init_motor(motor_ctx_t *m, int gpio, uint32_t baud)
{
    m->gpio = gpio;

    rmt_tx_channel_config_t tx_cfg = {
        .clk_src = RMT_CLK_SRC_DEFAULT,
        .gpio_num = gpio,
        .mem_block_symbols = 64,
        .resolution_hz = DSHOT_ESC_RESOLUTION_HZ,
        .trans_queue_depth = 4,
        .flags = { .with_dma = 0 },
    };
    ESP_ERROR_CHECK(rmt_new_tx_channel(&tx_cfg, &m->chan));

    dshot_esc_encoder_config_t enc_cfg = {
        .resolution = DSHOT_ESC_RESOLUTION_HZ,
        .baud_rate = 600000, // default DSHOT600
        .post_delay_us = 50,
    };
    ESP_ERROR_CHECK(rmt_new_dshot_esc_encoder(&enc_cfg, &m->enc600));
    m->active_enc = m->enc600; // fixed DSHOT600

    m->throttle.throttle = 0; // start stopped
    m->throttle.telemetry_req = false;
    m->current_pct = 0;
    m->target_pct = 0;
    m->last_dshot = 0;
    motor_apply(m);
}

static void motor_ramp_task(void *arg)
{
    (void)arg;
    while (1) {
        for (int i = 0; i < fan_count; ++i) {
            if (motors[i].current_pct != motors[i].target_pct) {
                int cur = motors[i].current_pct;
                int tgt = motors[i].target_pct;
                int delta = tgt - cur;
                if (delta > 0) cur += (delta > ramp_step_pct) ? ramp_step_pct : delta;
                else if (delta < 0) cur += (delta < -ramp_step_pct) ? -ramp_step_pct : delta;
                if (cur < 0) cur = 0;
                if (cur > 100) cur = 100;
                motors[i].current_pct = (uint8_t)cur;
                motors[i].throttle.throttle = pct_to_dshot((uint8_t)cur);
                motor_apply(&motors[i]);
            }
        }
        vTaskDelay(pdMS_TO_TICKS(ramp_tick_ms));
    }
}

void app_main(void)
{
    ESP_LOGI(TAG, "Init NVS");
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_LOGI(TAG, "Init Wi‑Fi");
    wifi_init_sta();

    // Wait until we have an IP before starting MQTT
    (void)xEventGroupWaitBits(s_wifi_event_group, WIFI_GOT_IP_BIT, pdFALSE, pdTRUE, portMAX_DELAY);

    ESP_LOGI(TAG, "Init modular config");
    parse_dshot_gpios();

    ESP_LOGI(TAG, "Init MQTT");
    mqtt_start();

    ESP_LOGI(TAG, "Init DShot motors (%d)", fan_count);
    for (int i = 0; i < fan_count; ++i) {
        dshot_init_motor(&motors[i], dshot_gpios[i], 600000);
        motors[i].throttle.throttle = pct_to_dshot(0);
        motor_apply(&motors[i]);
    }
    vTaskDelay(pdMS_TO_TICKS(1500));

    // Start ramp controller task (high-ish priority to keep updates smooth)
    xTaskCreatePinnedToCore(motor_ramp_task, "motor_ramp", 3072, NULL, 18, NULL, 0);
    // Telemetry removed: no telemetry task

    ESP_LOGI(TAG, "Setup complete");

    // Heartbeat task: periodically publish states
    while (1) {
        if (mqtt && s_mqtt_connected) {
            publish_states();
        }
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

// (Telemetry task removed)

