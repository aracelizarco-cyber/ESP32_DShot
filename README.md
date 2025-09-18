# Greenhouse ESP DShot Fan Controller (ESP32‑S3)

Standalone ESP‑IDF app to control multiple BLHeli/KISS ESCs over DShot using RMT. It exposes MQTT control with Home Assistant discovery for fan entities, smooth ramping, and comprehensive safety features. Configurable via Kconfig with secure defaults.

## Features
- **DShot Protocol**: Configurable DShot150/300/600/1200 via RMT with continuous stream for stable throttle
- **Multiple Fans**: Support for 1-8 fans with configurable GPIO mapping
- **MQTT Integration**: Full Home Assistant discovery with fan entities (on/off and percentage control)
- **Safety Systems**: 
  - Optional arming requirement (default: enabled)
  - Failsafe timeout protection (default: 1.5s)
  - Auto-disarm timeout (default: 5 minutes)
  - Command validation and authorization
- **Smooth Control**: Configurable ramping with step size and timing control
- **Network Resilience**: Per‑node availability using MAC suffix, robust Wi-Fi configuration
- **Secure Configuration**: No hardcoded secrets, safe defaults, NVS provisioning support

## Security Features
- **No Hardcoded Secrets**: Wi-Fi and MQTT credentials default to empty strings
- **NVS Provisioning**: Supports secure credential storage in NVS
- **Safe Defaults**: Default configuration uses placeholder IP and empty credentials
- **Arming System**: Fans cannot operate without explicit arming (configurable)
- **Failsafe Protection**: Automatic fan shutdown on communication loss

## Configure

### Basic Configuration
Run menuconfig to configure the system:
```bash
idf.py menuconfig
```

Navigate to "Greenhouse ESP DShot ESC Controller" for all settings:

#### Network Configuration
- **Wi-Fi SSID/Password**: Leave empty for NVS provisioning (recommended for security)
- **MQTT Broker URI**: Default `mqtt://10.10.10.1` (change to your broker)
- **MQTT Username/Password**: Leave empty if no authentication required

#### Fan Configuration  
- **Fan Count**: Number of fans (1-8, default: 2)
- **Fan Index Start**: Starting fan number for multi-node setups (default: 1)
- **DShot GPIOs**: Comma-separated GPIO list (default: "5,7")
- **Minimum Spin %**: Minimum percentage to prevent stalls (0-50%, default: 15%)
- **Ramp Settings**: Step size (1-20%, default: 3%) and timing (5-200ms, default: 20ms)

#### DShot Protocol
- **Speed**: DShot150/300/600/1200 (default: 600)
- **Telemetry**: Enable DShot telemetry (default: disabled)

#### Safety Settings
- **Failsafe Timeout**: Auto-stop timeout (100-5000ms, default: 1500ms)
- **Require Arming**: Fans need explicit arming (default: enabled)
- **Auto-disarm**: Automatic disarm timeout (60-3600s, default: 300s)

### Credential Management

#### Option 1: NVS Provisioning (Recommended)
```bash
# Use ESP-IDF provisioning tools or custom app to store credentials in NVS
# This keeps secrets out of firmware images
```

#### Option 2: Build-time Configuration (Less Secure)
```bash
idf.py menuconfig
# Set SSID/password in "Network Configuration" menu
# Note: This embeds credentials in firmware
```

## Build, Flash, Monitor

Prerequisites: ESP‑IDF v5.0+ installed

```bash
# Clean build with new configuration
idf.py fullclean
idf.py build

# Flash and monitor
idf.py -p /dev/ttyACM0 flash monitor
```

Replace `/dev/ttyACM0` with your serial port. Exit monitor with `Ctrl+]`.

## MQTT Topics and Safety

### Fan Control (per fan N)
- `greenhouse_esp/fanN/set` (ON/OFF) 
- `greenhouse_esp/fanN/state` (ON/OFF)
- `greenhouse_esp/fanN/percentage/set` (0‑100)
- `greenhouse_esp/fanN/percentage` (0‑100)

### Safety and System Control
- `greenhouse_esp/arm` (ARM/DISARM) - Enable/disable fan operation
- `greenhouse_esp/arm/state` (ARMED/DISARMED) - Current arm state  
- `greenhouse_esp/status/get` (any payload) - Request system status
- `greenhouse_esp/status` (JSON) - System status response
- `greenhouse_esp/status/MACADDR` (online/offline) - Node availability

### Status Response Format
```json
{
  "armed": true/false,
  "safe": true/false, 
  "uptime_s": 12345,
  "fan_count": 2,
  "failsafe_ms": 1500
}
```

### Home Assistant Discovery
- Automatic discovery published on MQTT connect
- Unique entity IDs using MAC address suffix
- Availability tracking per node
- Standard fan entity with on/off and percentage control

## Safety Operation

### Arming Sequence (if enabled)
1. **System starts DISARMED** - fans cannot operate
2. **Send ARM command**: Publish `ARM` to `greenhouse_esp/arm`
3. **System becomes ARMED** - fans can operate normally
4. **Auto-disarm**: System disarms after timeout (configurable)

### Failsafe Protection
- **Communication Loss**: Fans stop if no commands received within failsafe timeout
- **Safety Override**: All fans forced to idle if system becomes unsafe
- **Explicit Disarm**: Manual disarm immediately stops all fans

### Example Safety Commands
```bash
# Arm the system
mosquitto_pub -t greenhouse_esp/arm -m "ARM"

# Disarm the system (stops all fans)
mosquitto_pub -t greenhouse_esp/arm -m "DISARM"

# Check system status
mosquitto_pub -t greenhouse_esp/status/get -m "status"

# Control fans (only works when armed)
mosquitto_pub -t greenhouse_esp/fan1/set -m "ON"
mosquitto_pub -t greenhouse_esp/fan1/percentage/set -m "75"
```

## Configuration Files

- **`main/Kconfig`**: All configuration options with help text
- **`sdkconfig.defaults`**: Safe default values (no secrets)
- **`sdkconfig`**: Generated configuration (may contain secrets - add to `.gitignore`)

## Notes
- **Security**: Never commit `sdkconfig` with real credentials to version control
- **Telemetry**: DShot telemetry support present but not fully implemented
- **Multi-node**: Multiple ESP32s can control different fan ranges using `FAN_INDEX_START`
- **GPIO Validation**: Ensure selected GPIOs don't conflict with other peripherals
- **ESC Compatibility**: Tested with BLHeli and KISS ESCs, DShot600 recommended
