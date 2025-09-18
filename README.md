# Greenhouse ESP DShot Fan Controller (ESP32‑S3)

Standalone ESP‑IDF app to control multiple BLHeli/KISS ESCs over DShot600 using RMT. It exposes MQTT control with Home Assistant discovery for fan entities, smooth ramping, and comprehensive safety features.

## Features
- **Safety & Failsafe**: ARM/DISARM system with boot interlock, failsafe timeout, WiFi/MQTT disconnect protection
- **DShot600 via RMT**: Optimized transmission with precomputed frame cache and efficient RMT usage
- **Configurable**: N fans (menuconfig), GPIO list parsing, no hardcoded credentials
- **MQTT & Home Assistant**: Full discovery support with standard fan entities and system controls
- **Smooth ramping**: Configurable step and tick intervals with responsive UI updates
- **Per‑node availability**: Using MAC suffix for multi-device setups

## Safety Features
- **Boot Interlock**: System starts disarmed by default, requires explicit arming
- **Failsafe Timeout**: Automatically disarms if no commands received within configured time (default 1500ms)
- **Connectivity Protection**: Motors stop on WiFi or MQTT disconnection
- **Clean Shutdown**: RMT transmission completely stops when motors are off and system disarmed

## Configure
Run `idf.py menuconfig` to configure:

### Motor Configuration
- Number of fans and first fan index
- DShot GPIOs (comma‑separated)
- Minimum spin percentage
- Ramp step percentage and tick interval

### Safety Configuration  
- Failsafe timeout (500-10000ms, default 1500ms)
- Boot armed state (default: disarmed)

### Network Configuration
- WiFi SSID and password
- MQTT broker URI, username, and password

**Note**: No credentials are hardcoded. All must be configured via menuconfig.

## Build, Flash, Monitor

Prereq: ESP‑IDF installed in the dev container.

```bash
idf.py menuconfig  # Configure credentials and settings
idf.py fullclean build
idf.py -p /dev/ttyACM0 flash monitor
```

Replace the serial port as needed. Quit the monitor with Ctrl+] .

## MQTT/HA Topics

### System Control
- `greenhouse_esp/system/arm` - ARM/DISARM system (ON/OFF)
- `greenhouse_esp/system/state` - System state (ARMED/DISARMED)

### Per Fan N
- `greenhouse_esp/fanN/set` - Fan control (ON/OFF)
- `greenhouse_esp/fanN/state` - Fan state (ON/OFF)
- `greenhouse_esp/fanN/percentage/set` - Fan speed (0‑100)
- `greenhouse_esp/fanN/percentage` - Current fan speed (0‑100)

### Availability
- `greenhouse_esp/status/XXXXXX` - Node availability (online/offline)

Home Assistant discovery is published automatically on MQTT connect.

## Notes
- Telemetry is removed end‑to‑end. No RPM discovery or UART is present.
- The encoder remains DShot600 only.
- Frame cache optimizes DShot value computation for 0-100% range.
- System enforces safety: motors only run when armed, connected, and within failsafe timeout.
