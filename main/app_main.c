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
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

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

#include "dshot_esc_encoder.h"
#include "sdkconfig.h"

// Safety and failsafe configuration
#ifndef CONFIG_FAILSAFE_MS
#define CONFIG_FAILSAFE_MS 1500
#endif
#ifndef CONFIG_BOOT_ARMED
#define CONFIG_BOOT_ARMED false
#endif

// Modular configuration via Kconfig (see Kconfig file)
#ifndef CONFIG_FAN_COUNT
#define CONFIG_FAN_COUNT 2
#endif
#ifndef CONFIG_FAN_INDEX_START
#define CONFIG_FAN_INDEX_START 1
#endif
#ifndef CONFIG_DSHOT_GPIOS
#define CONFIG_DSHOT_GPIOS "5,7"
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
#ifndef CONFIG_MOTOR_POLE_PAIRS
#define CONFIG_MOTOR_POLE_PAIRS 7
#endif

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

// MQTT per-node namespace
static char s_mac_suffix[7] = {0};       // "A1B2C3"
static char s_base[64] = {0};            // "greenhouse_esp/A1B2C3"
static inline void load_mac_suffix_and_base(void) {
    if (s_mac_suffix[0] == 0) {
        uint8_t mac[6] = {0};
        esp_wifi_get_mac(WIFI_IF_STA, mac);
        snprintf(s_mac_suffix, sizeof(s_mac_suffix), "%02X%02X%02X", mac[3], mac[4], mac[5]);
        snprintf(s_base, sizeof(s_base), "greenhouse_esp/%s", s_mac_suffix);
    }
}

// Forward declaration
static void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);

typedef struct {
    rmt_channel_handle_t chan;
    rmt_encoder_handle_t enc600;
    rmt_encoder_handle_t active_enc;
    dshot_esc_throttle_t throttle;
    int gpio;
    bool active;
    bool stopped;  // true when RMT TX is completely stopped
    // Ramping/control state
    uint8_t current_pct;   // last applied percentage (0..100)
    uint8_t target_pct;    // desired percentage (0..100)
    uint8_t last_reported_pct; // last percentage published to MQTT (for UI responsiveness)
    uint32_t last_dshot;   // last applied raw DShot value + telemetry bit (composite)
    uint64_t last_tx_us;   // last time we kicked the TX (for watchdog refresh)
    uint32_t rmt_refresh_count; // count of periodic refreshes performed
} motor_ctx_t;

static int dshot_gpios[8] = {0};
static int fan_count = CONFIG_FAN_COUNT;
static int fan_index_start = CONFIG_FAN_INDEX_START;
static int min_spin_pct = CONFIG_MIN_SPIN_PCT;
static int ramp_step_pct = CONFIG_RAMP_STEP_PCT;
static int ramp_tick_ms = CONFIG_RAMP_TICK_MS;

static motor_ctx_t motors[8] = {0};
static esp_mqtt_client_handle_t mqtt = NULL;
// Fixed protocol: DSHOT600 only
static volatile bool s_mqtt_connected = false;

// Safety and failsafe state
static volatile bool s_system_armed = CONFIG_BOOT_ARMED;
static volatile uint64_t s_last_command_time = 0; // kept for potential future use; not used for failsafe
static volatile bool s_wifi_connected = false;

// DShot frame cache for optimization (precomputed 0-100%)
static uint16_t dshot_frame_cache[101] = {0};

// Trim helper
static inline void trim(char *s) {
    if (!s) return;
    // left trim
    while (*s == ' ' || *s == '\t') memmove(s, s+1, strlen(s));
    // right trim
    size_t n = strlen(s);
    while (n && (s[n-1] == ' ' || s[n-1] == '\t' || s[n-1] == '\r' || s[n-1] == '\n')) s[--n] = 0;
}

// Parse DSHOT GPIOs from config string
static void parse_dshot_gpios(void) {
    const char *str = CONFIG_DSHOT_GPIOS;
    char *copy = strdup(str ? str : "");
    if (!copy) {
        ESP_LOGE(TAG, "Out of memory parsing DSHOT_GPIOS");
        return;
    }
    char *saveptr = NULL;
    char *token = strtok_r(copy, ",", &saveptr);
    int idx = 0;
    while (token && idx < 8) {
        trim(token);
        dshot_gpios[idx++] = atoi(token);
        token = strtok_r(NULL, ",", &saveptr);
    }
    if (fan_count > idx) {
        ESP_LOGW(TAG, "FAN_COUNT=%d but only %d GPIOs provided; remaining motors will be disabled", fan_count, idx);
        fan_count = idx;
    }
    free(copy);
}

// Initialize DShot frame cache for optimized performance
static void init_dshot_frame_cache(void) {
    for (int pct = 0; pct <= 100; pct++) {
        if (pct == 0) {
            dshot_frame_cache[pct] = 48; // idle (we stop TX on 0%, so this is a fallback)
        } else {
            int effective_pct = pct;
            if (effective_pct < min_spin_pct) effective_pct = min_spin_pct;
            const uint16_t min = 48;
            const uint16_t max = 2047;
            uint32_t val = min + (uint32_t)(effective_pct) * (max - min) / 100U;
            if (val > max) val = max;
            dshot_frame_cache[pct] = (uint16_t)val;
        }
    }
}

// Safety check: returns true if motors should be allowed to run
static bool safety_check(void) {
    if (!s_system_armed) return false;
    if (!s_wifi_connected || !s_mqtt_connected) return false;
    return true;
}

// Helper: map 0-100% to DShot throttle range using cache
static uint16_t pct_to_dshot(uint8_t pct) {
    if (pct > 100) pct = 100;
    return dshot_frame_cache[pct];
}

// Stop motor RMT transmission completely
static void motor_stop(motor_ctx_t *m) {
    if (!m || !m->chan) return;
    if (m->active) {
        ESP_LOGI(TAG, "Stopping motor RMT TX on GPIO %d", m->gpio);
        (void)rmt_disable(m->chan);
        (void)rmt_tx_wait_all_done(m->chan, portMAX_DELAY);
        if (m->active_enc) {
            rmt_encoder_reset(m->active_enc);
        }
        m->active = false;
    }
    m->stopped = true;
}

// Apply current_pct to hardware; avoid churn at steady state
static void motor_apply(motor_ctx_t *m) {
    if (!m || !m->chan || (!m->enc600)) return;

    bool safe_to_spin = safety_check();
    uint64_t now = esp_timer_get_time();

    // Determine desired throttle: always send idle (48) at 0% to keep ESC happy
    uint16_t desired = 48;
    if (m->current_pct > 0 && safe_to_spin) {
        desired = pct_to_dshot(m->current_pct);
    } else {
        // Coerce state to idle if not safe
        if (m->current_pct > 0 && !safe_to_spin) {
            m->current_pct = 0;
            m->target_pct = 0;
        }
    }

    // If we were stopped but now need to run, mark
    if (m->stopped) {
        m->stopped = false;
    }

    // Update throttle value
    m->throttle.throttle = desired;
    m->throttle.telemetry_req = false;

    // Coerce illegal 0..47 to 48 to prevent DShot special commands
    if (m->throttle.throttle < 48) {
        m->throttle.throttle = 48;
    }

    uint32_t composite = ((uint32_t)m->throttle.throttle << 1) | (m->throttle.telemetry_req ? 1u : 0u);
    // 'now' computed earlier

    // Only re-arm TX when value changes or after a periodic refresh to keep timing stable
    bool value_changed = (composite != m->last_dshot);
    bool need_watchdog_refresh = (now - m->last_tx_us) > (1000ULL * 1000ULL); // 1s

    if (!value_changed && !need_watchdog_refresh) {
        return;
    }
    if (!value_changed && need_watchdog_refresh) {
        m->rmt_refresh_count++;
    }

    rmt_transmit_config_t tx_cfg = { .loop_count = -1 }; // continuous

    if (m->active) {
        (void)rmt_disable(m->chan);
        (void)rmt_tx_wait_all_done(m->chan, portMAX_DELAY);
        if (m->active_enc) {
            rmt_encoder_reset(m->active_enc);
        }
    }
    ESP_ERROR_CHECK(rmt_enable(m->chan));
    rmt_encoder_handle_t enc = m->active_enc ? m->active_enc : m->enc600;
    ESP_ERROR_CHECK(rmt_transmit(m->chan, enc, &m->throttle, sizeof(m->throttle), &tx_cfg));
    m->active = true;
    m->last_dshot = composite;
    m->last_tx_us = now;
}

static void ha_publish_discovery(void) {
    if (!mqtt) return;
    load_mac_suffix_and_base();

    // Availability topic per node
    char avail_topic[96];
    snprintf(avail_topic, sizeof(avail_topic), "%s/status", s_base);

    // Device object shared by all entities
    char device_obj[256];
    snprintf(device_obj, sizeof(device_obj),
             "\"device\":{"
               "\"identifiers\":[\"greenhouse_esp_%s\"],"
               "\"manufacturer\":\"Greenhouse\","
               "\"model\":\"ESP32 DShot Fan Ctrl\","
               "\"sw_version\":\"1.0.0\""
             "}",
             s_mac_suffix);

    // Publish system arm/disarm switch discovery
    char sys_disc[160];
    char sys_uniq[80];
    snprintf(sys_disc, sizeof(sys_disc), "homeassistant/switch/greenhouse_esp_system_%s/config", s_mac_suffix);
    snprintf(sys_uniq, sizeof(sys_uniq), "greenhouse_esp_system_%s", s_mac_suffix);

        int sys_len = snprintf(NULL, 0,
                         "{"
                             "\"name\":\"Greenhouse System Arm\"," 
                             "\"unique_id\":\"%s\"," 
                             "\"command_topic\":\"%s/system/arm\"," 
                             "\"state_topic\":\"%s/system/state\"," 
                             "\"payload_on\":\"ON\",\"payload_off\":\"OFF\"," 
                             "\"state_on\":\"ARMED\",\"state_off\":\"DISARMED\"," 
                             "\"availability_topic\":\"%s\"," 
                             "\"payload_available\":\"online\",\"payload_not_available\":\"offline\"," 
                             "\"icon\":\"mdi:shield-check\"," 
                             "%s"
                         "}",
                         sys_uniq, s_base, s_base, avail_topic, device_obj);
        if (sys_len > 0) {
                char *sys_payload = (char *)malloc((size_t)sys_len + 1);
                if (sys_payload) {
                        (void)snprintf(sys_payload, (size_t)sys_len + 1,
                         "{"
                             "\"name\":\"Greenhouse System Arm\"," 
                             "\"unique_id\":\"%s\"," 
                             "\"command_topic\":\"%s/system/arm\"," 
                             "\"state_topic\":\"%s/system/state\"," 
                             "\"payload_on\":\"ON\",\"payload_off\":\"OFF\"," 
                             "\"state_on\":\"ARMED\",\"state_off\":\"DISARMED\"," 
                             "\"availability_topic\":\"%s\"," 
                             "\"payload_available\":\"online\",\"payload_not_available\":\"offline\"," 
                             "\"icon\":\"mdi:shield-check\"," 
                             "%s"
                         "}",
                         sys_uniq, s_base, s_base, avail_topic, device_obj);
                        esp_mqtt_client_publish(mqtt, sys_disc, sys_payload, 0, 1, true);
                        free(sys_payload);
                }
        }

    // Fan entities
    for (int i = 0; i < fan_count; ++i) {
        int fan_idx = fan_index_start + i;

        char disc[192];
        char uniq[96];
        snprintf(disc, sizeof(disc), "homeassistant/fan/greenhouse_esp_motor%d_%s/config", fan_idx, s_mac_suffix);
        snprintf(uniq, sizeof(uniq), "greenhouse_esp_motor%d_%s", fan_idx, s_mac_suffix);

                int fan_len = snprintf(NULL, 0,
                                 "{"
                                     "\"name\":\"Tent Fan %d\"," 
                                     "\"unique_id\":\"%s\"," 
                                     "\"command_topic\":\"%s/fan/%d/set\"," 
                                     "\"state_topic\":\"%s/fan/%d/state\"," 
                                     "\"percentage_command_topic\":\"%s/fan/%d/percentage/set\"," 
                                     "\"percentage_state_topic\":\"%s/fan/%d/percentage\"," 
                                     "\"payload_on\":\"ON\",\"payload_off\":\"OFF\"," 
                                     "\"availability_topic\":\"%s\"," 
                                     "\"payload_available\":\"online\",\"payload_not_available\":\"offline\"," 
                                     "%s"
                                 "}",
                                 fan_idx, uniq,
                                 s_base, fan_idx,
                                 s_base, fan_idx,
                                 s_base, fan_idx,
                                 s_base, fan_idx,
                                 avail_topic, device_obj);
                if (fan_len > 0) {
                        char *payload = (char *)malloc((size_t)fan_len + 1);
                        if (payload) {
                                (void)snprintf(payload, (size_t)fan_len + 1,
                                 "{"
                                     "\"name\":\"Tent Fan %d\"," 
                                     "\"unique_id\":\"%s\"," 
                                     "\"command_topic\":\"%s/fan/%d/set\"," 
                                     "\"state_topic\":\"%s/fan/%d/state\"," 
                                     "\"percentage_command_topic\":\"%s/fan/%d/percentage/set\"," 
                                     "\"percentage_state_topic\":\"%s/fan/%d/percentage\"," 
                                     "\"payload_on\":\"ON\",\"payload_off\":\"OFF\"," 
                                     "\"availability_topic\":\"%s\"," 
                                     "\"payload_available\":\"online\",\"payload_not_available\":\"offline\"," 
                                     "%s"
                                 "}",
                                 fan_idx, uniq,
                                 s_base, fan_idx,
                                 s_base, fan_idx,
                                 s_base, fan_idx,
                                 s_base, fan_idx,
                                 avail_topic, device_obj);
                                esp_mqtt_client_publish(mqtt, disc, payload, 0, 1, true);
                                free(payload);
                        }
                }
    }

    // Set node online
    esp_mqtt_client_publish(mqtt, avail_topic, "online", 0, 1, true);
}

static void publish_states(void) {
    if (!mqtt || !s_mqtt_connected) return;
    load_mac_suffix_and_base();

    // Publish system status (non-retained for UI responsiveness)
    char sys_topic[96];
    snprintf(sys_topic, sizeof(sys_topic), "%s/system/state", s_base);
    esp_mqtt_client_publish(mqtt, sys_topic, s_system_armed ? "ARMED" : "DISARMED", 0, 0, false);

    char buf[16];
    for (int i = 0; i < fan_count; ++i) {
        int fan_idx = fan_index_start + i;
        uint8_t pct = motors[i].current_pct;
        const char *state = (pct > 0) ? "ON" : "OFF";
        char topic_state[96];
        char topic_pct[96];
        snprintf(topic_state, sizeof(topic_state), "%s/fan/%d/state", s_base, fan_idx);
        snprintf(topic_pct, sizeof(topic_pct), "%s/fan/%d/percentage", s_base, fan_idx);
        // retained true for steady UI
        esp_mqtt_client_publish(mqtt, topic_state, state, 0, 1, true);
        snprintf(buf, sizeof(buf), "%u", pct);
        esp_mqtt_client_publish(mqtt, topic_pct, buf, 0, 1, true);
    }
}

// Publish a single fan's state/percentage quickly (non-retained) for UI responsiveness
static void publish_one_state_fast(int idx) {
    if (!mqtt || !s_mqtt_connected) return;
    if (idx < 0 || idx >= fan_count) return;
    load_mac_suffix_and_base();

    int fan_idx = fan_index_start + idx;
    uint8_t pct = motors[idx].current_pct;
    const char *state = (pct > 0) ? "ON" : "OFF";
    char topic_state[96];
    char topic_pct[96];
    char buf[16];
    snprintf(topic_state, sizeof(topic_state), "%s/fan/%d/state", s_base, fan_idx);
    snprintf(topic_pct, sizeof(topic_pct), "%s/fan/%d/percentage", s_base, fan_idx);
    esp_mqtt_client_publish(mqtt, topic_state, state, 0, 0, false);
    snprintf(buf, sizeof(buf), "%u", pct);
    esp_mqtt_client_publish(mqtt, topic_pct, buf, 0, 0, false);
}

static void publish_metrics(void) {
    if (!mqtt || !s_mqtt_connected) return;
    load_mac_suffix_and_base();

    char buf[16];
    for (int i = 0; i < fan_count; ++i) {
        int fan_idx = fan_index_start + i;
        char topic_ref[128];
        snprintf(topic_ref, sizeof(topic_ref), "%s/fan/%d/rmt_refresh_count", s_base, fan_idx);
        snprintf(buf, sizeof(buf), "%" PRIu32, motors[i].rmt_refresh_count);
        esp_mqtt_client_publish(mqtt, topic_ref, buf, 0, 1, true);
    }
}

static void mqtt_on_message(const char *topic, const char *data, int len) {
    char dbg_payload[64] = {0};
    int dbg_len = len < (int)sizeof(dbg_payload)-1 ? len : (int)sizeof(dbg_payload)-1;
    memcpy(dbg_payload, data, dbg_len);
    ESP_LOGI(TAG, "MQTT RX topic='%s' payload='%s'", topic, dbg_payload);

    load_mac_suffix_and_base();

    // Command timestamp no longer used for failsafe

    // Handle system arm/disarm commands
    char sys_arm_topic[96];
    snprintf(sys_arm_topic, sizeof(sys_arm_topic), "%s/system/arm", s_base);
    if (strcmp(topic, sys_arm_topic) == 0) {
        if (len >= 2 && strncasecmp(data, "ON", 2) == 0) {
            s_system_armed = true;
            ESP_LOGI(TAG, "System ARMED");
        } else if (len >= 3 && strncasecmp(data, "OFF", 3) == 0) {
            s_system_armed = false;
            ESP_LOGI(TAG, "System DISARMED");
            // Stop all motors when disarmed
            for (int i = 0; i < fan_count; ++i) {
                motors[i].target_pct = 0;
            }
        }
        publish_states(); // update retained DISARMED/ARMED immediately
        return;
    }

    for (int i = 0; i < fan_count; ++i) {
        int fan_idx = fan_index_start + i;
        char set_topic[96];
        char pct_set_topic[96];
        snprintf(set_topic, sizeof(set_topic), "%s/fan/%d/set", s_base, fan_idx);
        snprintf(pct_set_topic, sizeof(pct_set_topic), "%s/fan/%d/percentage/set", s_base, fan_idx);

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

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    esp_mqtt_event_handle_t event = (esp_mqtt_event_handle_t)event_data;
    switch (event_id) {
    case MQTT_EVENT_CONNECTED: {
        ESP_LOGI(TAG, "MQTT connected");
        s_mqtt_connected = true;
        mqtt = event->client;
        load_mac_suffix_and_base();

        // Subscribe to system arm topic
        char sys_arm_topic[96];
        snprintf(sys_arm_topic, sizeof(sys_arm_topic), "%s/system/arm", s_base);
        esp_mqtt_client_subscribe(event->client, sys_arm_topic, 1);

        // Subscribe to control topics per fan
        for (int i = 0; i < fan_count; ++i) {
            int fan_idx = fan_index_start + i;
            char set_topic[96];
            char pct_set_topic[96];
            snprintf(set_topic, sizeof(set_topic), "%s/fan/%d/set", s_base, fan_idx);
            snprintf(pct_set_topic, sizeof(pct_set_topic), "%s/fan/%d/percentage/set", s_base, fan_idx);
            esp_mqtt_client_subscribe(event->client, set_topic, 1);
            esp_mqtt_client_subscribe(event->client, pct_set_topic, 1);
        }
        // Publish discovery and initial states
        ha_publish_discovery();
        publish_states();
        break;
    }
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGW(TAG, "MQTT disconnected - ramping motors to 0 for safety");
        s_mqtt_connected = false;
        for (int i = 0; i < fan_count; ++i) motors[i].target_pct = 0;
        break;
    case MQTT_EVENT_DATA: {
        char topic[256];
        int tlen = event->topic_len;
        if (tlen >= (int)sizeof(topic)) tlen = sizeof(topic) - 1;
        memcpy(topic, event->topic, tlen);
        topic[tlen] = 0;

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

static void wifi_init_sta(void) {
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
    wifi_config.sta.threshold.authmode = WIFI_AUTH_WPA_WPA2_PSK;
    wifi_config.sta.sae_pwe_h2e = WPA3_SAE_PWE_BOTH;
    wifi_config.sta.pmf_cfg.required = false; // PMF optional
    wifi_config.sta.scan_method = WIFI_ALL_CHANNEL_SCAN;
    wifi_config.sta.sort_method = WIFI_CONNECT_AP_BY_SIGNAL;

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    // Improve latency/reliability of TCP/MQTT by disabling Wi‑Fi power save
    ESP_ERROR_CHECK(esp_wifi_set_ps(WIFI_PS_NONE));
    // Boost TX power for link robustness (units: 0.25 dBm, 84 => 21 dBm max)
    (void)esp_wifi_set_max_tx_power(84);

    ESP_LOGI(TAG, "wifi_init_sta finished, waiting for IP...");
}

static void mqtt_start(void) {
    load_mac_suffix_and_base();

    // Unique client_id from MAC
    static char client_id[32];
    snprintf(client_id, sizeof(client_id), "greenhouse_esp_%s", s_mac_suffix);

    char avail_topic[96];
    snprintf(avail_topic, sizeof(avail_topic), "%s/status", s_base);

    esp_mqtt_client_config_t cfg = {
        .broker.address.uri = CONFIG_MQTT_BROKER_URI, // do not override port; supports mqtt:// or mqtts://
        .credentials.username = CONFIG_MQTT_USERNAME,
        .credentials.authentication.password = CONFIG_MQTT_PASSWORD,
        .credentials.client_id = client_id,
        .session.keepalive = 30,
        .network.reconnect_timeout_ms = 3000,
        .network.timeout_ms = 5000,
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

static void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {
    if (event_base == WIFI_EVENT) {
        switch (event_id) {
        case WIFI_EVENT_STA_START:
            ESP_LOGI(TAG, "Wi‑Fi start, connecting...");
            esp_wifi_connect();
            break;
        case WIFI_EVENT_STA_CONNECTED:
            ESP_LOGI(TAG, "Wi‑Fi connected to AP");
            s_wifi_connected = true;
            xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
            break;
        case WIFI_EVENT_STA_DISCONNECTED: {
            wifi_event_sta_disconnected_t *disc = (wifi_event_sta_disconnected_t *)event_data;
            const char *reason_str = "";
            if (disc) {
                switch (disc->reason) {
                    case 1: reason_str = "UNSPECIFIED"; break;
                    case 2: reason_str = "AUTH_EXPIRE"; break;
                    case 3: reason_str = "AUTH_LEAVE"; break;
                    case 4: reason_str = "ASSOC_EXPIRE"; break;
                    case 5: reason_str = "ASSOC_TOOMANY"; break;
                    case 6: reason_str = "NOT_AUTHED"; break;
                    case 7: reason_str = "NOT_ASSOCED"; break;
                    case 8: reason_str = "ASSOC_LEAVE"; break;
                    case 9: reason_str = "ASSOC_NOT_AUTHED"; break;
                    case 201: reason_str = "NO_AP_FOUND"; break;
                    case 202: reason_str = "AUTH_FAIL"; break;
                    case 203: reason_str = "ASSOC_FAIL"; break;
                    case 204: reason_str = "HANDSHAKE_TIMEOUT"; break;
                    case 205: reason_str = "CONNECTION_FAIL"; break;
                    default: reason_str = "OTHER"; break;
                }
            }
            ESP_LOGW(TAG, "Wi‑Fi disconnected, reason=%d (%s), retrying...", disc ? disc->reason : -1, reason_str);
            s_wifi_connected = false;
            // Safety: stop all motors on WiFi disconnect
            for (int i = 0; i < fan_count; ++i) {
                motors[i].target_pct = 0;
            }
            esp_wifi_connect();
            xEventGroupClearBits(s_wifi_event_group, WIFI_CONNECTED_BIT | WIFI_GOT_IP_BIT);
            break; }
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

static void dshot_init_motor(motor_ctx_t *m, int gpio, uint32_t baud) {
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
        .baud_rate = 600000, // DSHOT600
        .post_delay_us = 100,
    };
    ESP_ERROR_CHECK(rmt_new_dshot_esc_encoder(&enc_cfg, &m->enc600));
    m->active_enc = m->enc600; // fixed DSHOT600

    m->throttle.throttle = 0; // start stopped
    m->throttle.telemetry_req = false;
    m->current_pct = 0;
    m->target_pct = 0;
    m->last_reported_pct = 255; // force first publish on change
    m->last_dshot = 0;
    m->last_tx_us = 0;
    m->rmt_refresh_count = 0;
    m->stopped = true; // start with RMT TX stopped
}

static void motor_ramp_task(void *arg) {
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
                // Publish promptly when percentage changes meaningfully or crosses ON/OFF
                uint8_t last = motors[i].last_reported_pct;
                bool crossing_on = (last == 0 && motors[i].current_pct > 0);
                bool crossing_off = (last > 0 && motors[i].current_pct == 0);
                bool reached_target = (motors[i].current_pct == motors[i].target_pct);
                int diff = (int)motors[i].current_pct - (int)last;
                if (diff < 0) diff = -diff;
                bool step_change = (last == 255) || (diff >= 5);
                if (crossing_on || crossing_off || step_change || reached_target) {
                    publish_one_state_fast(i);
                    motors[i].last_reported_pct = motors[i].current_pct;
                }
            } else {
                // Even if target is steady, avoid churn; a rare watchdog refresh happens in motor_apply
                motor_apply(&motors[i]);
            }
        }
        vTaskDelay(pdMS_TO_TICKS(ramp_tick_ms));
    }
}

void app_main(void) {
    ESP_LOGI(TAG, "Init NVS");
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_LOGI(TAG, "Init modular config");
    parse_dshot_gpios();

    // Initialize DShot frame cache for optimized performance
    ESP_LOGI(TAG, "Init DShot frame cache");
    init_dshot_frame_cache();

    ESP_LOGI(TAG, "Init Wi‑Fi");
    wifi_init_sta();

    // Wait until we have an IP before starting MQTT
    (void)xEventGroupWaitBits(s_wifi_event_group, WIFI_GOT_IP_BIT, pdFALSE, pdTRUE, portMAX_DELAY);

    ESP_LOGI(TAG, "Init MQTT");
    mqtt_start();

    ESP_LOGI(TAG, "Init DShot motors (%d) - starting %s", fan_count, s_system_armed ? "ARMED" : "DISARMED");
    for (int i = 0; i < fan_count; ++i) {
        dshot_init_motor(&motors[i], dshot_gpios[i], 600000);
    }

    // Initialize last command time
    s_last_command_time = esp_timer_get_time();

    // Start ramp controller task (high-ish priority to keep updates smooth)
    xTaskCreatePinnedToCore(motor_ramp_task, "motor_ramp", 3072, NULL, 18, NULL, 0);

    ESP_LOGI(TAG, "Setup complete - system %s", s_system_armed ? "ARMED" : "DISARMED");

    // Heartbeat task: periodically publish states and metrics
    while (1) {
        if (mqtt && s_mqtt_connected) {
            publish_states();
            publish_metrics();
        }
        // Failsafe is enforced in motor_apply
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}