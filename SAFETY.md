# Safety Features Guide

## Overview
The ESP32 DShot Greenhouse Controller implements comprehensive safety features to prevent accidental motor operation and ensure safe operation in all conditions.

## Safety Features

### 1. ARM/DISARM System
- System starts in **DISARMED** state by default (configurable via `CONFIG_BOOT_ARMED`)
- Motors will not spin unless explicitly armed
- Can be controlled via MQTT: `greenhouse_esp/system/arm` (ON/OFF)
- System state published to: `greenhouse_esp/system/state` (ARMED/DISARMED)

### 2. Failsafe Timeout
- Configurable timeout (default 1500ms via `CONFIG_FAILSAFE_MS`)
- If no valid command received within timeout, system automatically disarms
- Motors ramp down to 0% and RMT transmission stops
- Prevents runaway motors if controller loses communication

### 3. Connectivity Protection
- **WiFi Disconnect**: All motors stop immediately when WiFi connection lost
- **MQTT Disconnect**: All motors stop and availability goes offline
- System only operates when both WiFi and MQTT are connected

### 4. Clean Motor Shutdown
- When motors reach 0% and system is disarmed, RMT transmission completely stops
- No continuous streaming of idle values (more efficient and safer)
- Motors cleanly re-start when needed

## Configuration

### Via menuconfig (`idf.py menuconfig`)

#### Safety Settings
```
Greenhouse ESP DShot ESC Controller  --->
    [*] Boot in armed state           # Default: disabled for safety
    (1500) Failsafe timeout (ms)     # Range: 500-10000ms
```

#### Network Settings (Required)
```
Greenhouse ESP DShot ESC Controller  --->
    () Wi‑Fi SSID                     # Must be configured
    () Wi‑Fi Password                 # Must be configured
    (mqtt://192.168.1.100) MQTT broker URI
    (greenhouse_esp) MQTT username
    () MQTT password                  # Must be configured
```

## Usage Examples

### Initial Setup
1. Configure credentials: `idf.py menuconfig`
2. Build and flash: `idf.py build flash monitor`
3. System starts DISARMED - motors won't spin
4. Arm system via MQTT or Home Assistant

### MQTT Commands
```bash
# Arm the system
mosquitto_pub -h broker -t "greenhouse_esp/system/arm" -m "ON"

# Check system status
mosquitto_sub -h broker -t "greenhouse_esp/system/state"

# Control fan 1
mosquitto_pub -h broker -t "greenhouse_esp/fan1/set" -m "ON"
mosquitto_pub -h broker -t "greenhouse_esp/fan1/percentage/set" -m "50"

# Emergency stop - disarm system
mosquitto_pub -h broker -t "greenhouse_esp/system/arm" -m "OFF"
```

### Home Assistant
The system automatically publishes discovery information:
- **System Arm Switch**: Control system armed state
- **Fan Entities**: Standard MQTT fans with percentage control
- **Availability**: Shows online/offline status

## Safety Behavior

| Condition | Behavior |
|-----------|----------|
| Boot | System DISARMED, motors stopped |
| WiFi Lost | Motors stop immediately, system stays armed |
| MQTT Lost | Motors stop, availability offline |
| No Commands | Auto-disarm after failsafe timeout |
| Manual Disarm | Motors ramp to 0% and stop |
| Fan 0% + Disarmed | RMT transmission stops completely |

## Troubleshooting

### Motors Won't Start
1. Check system is ARMED: `greenhouse_esp/system/state`
2. Verify WiFi/MQTT connection
3. Check failsafe timeout hasn't triggered
4. Ensure fan percentage > 0

### Unexpected Motor Stop
1. Check logs for disconnect messages
2. Verify command frequency < failsafe timeout
3. Check WiFi signal strength
4. Verify MQTT broker connectivity

### System Won't Arm
1. Verify MQTT connection established
2. Check command topic: `greenhouse_esp/system/arm`
3. Ensure payload is exactly "ON"
4. Check device logs for errors

## Log Messages

Key safety log messages to watch for:
```
[greenhouse] System ARMED
[greenhouse] System DISARMED  
[greenhouse] WARN: Failsafe timeout triggered, disarming system
[greenhouse] WARN: MQTT disconnected - stopping all motors for safety
[greenhouse] WARN: Wi‑Fi disconnected, reason=X, retrying...
[greenhouse] Stopping motor RMT TX on GPIO X
```

## Best Practices

1. **Always configure failsafe timeout** appropriately for your application
2. **Use reliable WiFi** - unstable connections cause frequent motor stops
3. **Monitor system status** in Home Assistant or via MQTT
4. **Test emergency scenarios** during setup
5. **Keep command frequency** higher than failsafe timeout for continuous operation