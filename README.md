# Greenhouse ESP DShot Fan Controller (ESP32‑S3)

Standalone ESP‑IDF app to control multiple BLHeli/KISS ESCs over DShot600 using RMT. It exposes MQTT control with Home Assistant discovery for fan entities and smooth ramping. Telemetry is intentionally removed for now.

## Features
- DShot600 via RMT, continuous stream for stable throttle
- N fans (menuconfig), GPIO list parsing (one GPIO per fan)
- MQTT control and HA discovery per fan: on/off and percentage
- Smooth ramping with configurable step and tick
- Per‑node availability using MAC suffix

## Configure
Run menuconfig to set pins and behavior:
- Number of fans and first fan index
- DShot GPIOs (comma‑separated)
- Minimum spin percentage
- Ramp step percentage and tick interval

## Build, Flash, Monitor

Prereq: ESP‑IDF installed in the dev container (already present here).

```bash
idf.py fullclean build
idf.py -p /dev/ttyACM0 flash monitor
```

Replace the serial port as needed. Quit the monitor with Ctrl+] .

## MQTT/HA
- Broker URI, username, and password are defined in `main/app_main.c`.
- Topics per fan N:
   - greenhouse_esp/fanN/set (ON/OFF)
   - greenhouse_esp/fanN/state (ON/OFF)
   - greenhouse_esp/fanN/percentage/set (0‑100)
   - greenhouse_esp/fanN/percentage (0‑100)
- HA discovery is published automatically on connect.

## Notes
- Telemetry is removed end‑to‑end. No RPM discovery or UART is present.
- The encoder remains DShot600 only.
