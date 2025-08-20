PS C:\Users\Carlo\workspace\dshot_esc> $env:IDF_PATH = 'C:\Users\Carlo\v5.5\esp-idf';
PS C:\Users\Carlo\workspace\dshot_esc>  & 'C:\Users\Carlo\.espressif\python_env\idf5.5_py3.11_env\Scripts\python.exe' 'C:\Users\Carlo\v5.5\esp-idf\tools\idf_monitor.py' -p COM6 -b 115200 --toolchain-prefix xtensa-esp32s3-elf- --make '''C:\Users\Carlo\.espressif\python_env\idf5.5_py3.11_env\Scripts\python.exe'' ''C:\Users\Carlo\v5.5\esp-idf\tools\idf.py''' --target esp32s3 'c:\Users\Carlo\workspace\dshot_esc\build\dshot_esc.elf'
--- Warning: GDB cannot open serial ports accessed as COMx
--- Using \\.\COM6 instead...
--- esp-idf-monitor 1.7.0 on \\.\COM6 115200        
--- Quit: Ctrl+] | Menu: Ctrl+T | Help: Ctrl+T followed by Ctrl+H
I (1672) esp_netif_handlers: sta ip: 192.168.53.147, mask: 255.2ESP-ROM:esp32s3-20210327
Build:Mar 27 2021
rst:0x15 (USB_UART_CHIP_RESET),boot:0x28 (SPI_FAST_FLASH_BOOT)
Saved PC:0x4037a88e
--- 0x4037a88e: esp_cpu_wait_for_intr at /opt/esp/idf/components/esp_hw_support/cpu.c:64
SPIWP:0xee
mode:DIO, clock div:1
load:0x3fce2820,len:0x1588
load:0x403c8700,len:0xd20
load:0x403cb700,len:0x2f2c
entry 0x403c8924
I (24) boot: ESP-IDF v6.0-dev-1489-g4e036983a7 2nd stage bootloader
I (25) boot: compile time Aug 19 2025 21:57:35      
I (25) boot: Multicore bootloader
I (26) boot: chip revision: v0.2
I (29) boot: efuse block revision: v1.3
I (33) boot.esp32s3: Boot SPI Speed : 80MHz
I (37) boot.esp32s3: SPI Mode       : DIO
I (40) boot.esp32s3: SPI Flash Size : 2MB
I (44) boot: Enabling RNG early entropy source...
I (49) boot: Partition Table:
I (51) boot: ## Label            Usage          Type ST Offset   Length
I (58) boot:  0 nvs              WiFi data        01 02 00009000 00006000
I (64) boot:  1 phy_init         RF data          01 01 0000f000 00001000
I (71) boot:  2 factory          factory app      00 00 00010000 00100000
I (77) boot: End of partition table
I (81) esp_image: segment 0: paddr=00010020 vaddr=3c0b0020 size=20b58h (133976) map
I (111) esp_image: segment 1: paddr=00030b80 vaddr=3fc9b700 size=04cb0h ( 19632) load
I (116) esp_image: segment 2: paddr=00035838 vaddr=40374000 size=0a7e0h ( 42976) load
I (126) esp_image: segment 3: paddr=00040020 vaddr=42000020 size=a01e0h (655840) map
I (242) esp_image: segment 4: paddr=000e0208 vaddr=4037e7e0 size=0cef4h ( 52980) load
I (254) esp_image: segment 5: paddr=000ed104 vaddr=50000000 size=00020h (    32) load
I (265) boot: Loaded app from partition at offset 0x10000
I (265) boot: Disabling RNG early entropy source... 
I (275) cpu_start: Multicore app
I (283) cpu_start: GPIO 44 and 43 are used as console UART I/O pins
I (284) cpu_start: Pro cpu start user code
I (284) cpu_start: cpu freq: 160000000 Hz
I (286) app_init: Application information:
I (290) app_init: Project name:     dshot_esc
I (294) app_init: App version:      1
I (297) app_init: Compile time:     Aug 19 2025 21:57:23
I (302) app_init: ELF file SHA256:  7b441f547...    
I (306) app_init: ESP-IDF:          v6.0-dev-1489-g4e036983a7
I (312) efuse_init: Min chip rev:     v0.0
I (316) efuse_init: Max chip rev:     v0.99
I (320) efuse_init: Chip rev:         v0.2
I (324) heap_init: Initializing. RAM available for dynamic allocation:
I (330) heap_init: At 3FCA4738 len 00044FD8 (275 KiB): RAM
I (335) heap_init: At 3FCE9710 len 00005724 (21 KiB): RAM
I (340) heap_init: At 3FCF0000 len 00008000 (32 KiB): DRAM
I (345) heap_init: At 600FE000 len 00001FE8 (7 KiB): RTCRAM
I (352) spi_flash: detected chip: generic
I (354) spi_flash: flash io: dio
W (357) spi_flash: Detected size(4096k) larger than the size in the binary image header(2048k). Using the size in the binary image header.
I (370) sleep_gpio: Configure to isolate all GPIO pins in sleep state
I (376) sleep_gpio: Enable automatic switching of GPIO sleep configuration
I (383) main_task: Started on CPU0
I (393) main_task: Calling app_main()
I (393) greenhouse: Init NVS
I (413) greenhouse: Init Wi‑Fi
I (413) pp: pp rom version: e7ae62f
I (413) net80211: net80211 rom version: e7ae62f     
I (423) wifi:wifi driver task: 3fced480, prio:23, stack:6656, core=0
I (433) wifi:wifi firmware version: 5984c42
I (433) wifi:wifi certification version: v7.0       
I (433) wifi:config NVS flash: enabled
I (433) wifi:config nano formatting: disabled
I (433) wifi:Init data frame dynamic rx buffer num: 32
I (443) wifi:Init static rx mgmt buffer num: 5      
I (443) wifi:Init management short buffer num: 32   
I (443) wifi:Init dynamic tx buffer num: 32
I (453) wifi:Init static tx FG buffer num: 2        
I (453) wifi:Init static rx buffer size: 1600       
I (463) wifi:Init static rx buffer num: 10
I (463) wifi:Init dynamic rx buffer num: 32
I (473) wifi_init: rx ba win: 6
I (473) wifi_init: accept mbox: 6
I (473) wifi_init: tcpip mbox: 32
I (473) wifi_init: udp mbox: 6
I (483) wifi_init: tcp mbox: 6
I (483) wifi_init: tcp tx win: 5760
I (483) wifi_init: tcp rx win: 5760
I (493) wifi_init: tcp mss: 1440
I (493) wifi_init: WiFi IRAM OP enabled
I (493) wifi_init: WiFi RX IRAM OP enabled
I (503) phy_init: phy_version 701,f4f1da3a,Mar  3 2025,15:50:10
I (543) wifi:mode : sta (f0:f5:bd:79:1b:a0)
I (543) wifi:enable tsf
I (543) greenhouse: wifi_init_sta finished, waiting for IP...
I (543) greenhouse: Wi‑Fi start, connecting...      

I (553) wifi:new:<10,0>, old:<1,0>, ap:<255,255>, sta:<10,0>, prof:1, snd_ch_cfg:0x0
I (553) wifi:state: init -> auth (0xb0)
I (563) wifi:state: auth -> assoc (0x0)
I (573) wifi:state: assoc -> run (0x10)
I (613) wifi:connected with Tent, aid = 37, channel 10, BW20, bssid = 82:9c:25:89:44:45
I (613) wifi:security: WPA2-PSK, phy: bgn, rssi: -33
I (623) wifi:pm start, type: 1

I (623) wifi:dp: 1, bi: 102400, li: 3, scale listen interval from 307200 us to 307200 us
I (633) wifi:set rx beacon pti, rx_bcn_pti: 0, bcn_timeout: 25000, mt_pti: 0, mt_time: 10000
I (643) greenhouse: Wi‑Fi connected to AP
I (643) wifi:<ba-add>idx:0 (ifx:0, 82:9c:25:89:44:45), tid:7, ssn:3, winSize:64
I (693) wifi:AP's beacon interval = 102400 us, DTIM period = 1
I (1653) esp_netif_handlers: sta ip: 192.168.53.147, mask: 255.255.255.0, gw: 192.168.53.1
I (1653) greenhouse: Got IP: 192.168.53.147, GW: 192.168.53.1, Netmask: 255.255.255.0
I (1653) greenhouse: Init MQTT
I (1663) greenhouse: Init DShot motors
E (1663) rmt: rmt_tx_enable(758): channel not in init state
ESP_ERROR_CHECK failed: esp_err_t 0x103 (ESP_ERR_INVALID_STATE) at 0x4200c190
--- 0x4200c190: motor_apply at /workspaces/dshot_esc/main/app_main.c:107
file: "./main/app_main.c" line 107
func: motor_apply
expression: rmt_enable(m->chan)

abort() was called at PC 0x4037ce37 on core 0       
--- 0x4037ce37: _esp_error_check_failed at /opt/esp/idf/components/esp_system/esp_err.c:49


Backtrace: 0x4037ce79:0x3fca80f0 0x4037ce41:0x3fca8110 0x40385139:0x3fca8130 0x4037ce37:0x3fca81a0 0x4200c190:0x3fca81d0 0x4200c65f:0x3fca8200 0x4200c737:0x3fca8250 0x4209f188:0x3fca8290 0x4037db31:0x3fca82c0
--- 0x4037ce79: panic_abort at /opt/esp/idf/components/esp_system/panic.c:469
--- 0x4037ce41: esp_system_abort at /opt/esp/idf/components/esp_system/port/esp_system_chip.c:87        
--- 0x40385139: abort at /opt/esp/idf/components/newlib/src/abort.c:38
--- 0x4037ce37: _esp_error_check_failed at /opt/esp/idf/components/esp_system/esp_err.c:49
--- 0x4200c190: motor_apply at /workspaces/dshot_esc/main/app_main.c:107
--- 0x4200c65f: dshot_init_motor at /workspaces/dshot_esc/main/app_main.c:446
--- 0x4200c737: app_main at /workspaces/dshot_esc/main/app_main.c:469
--- 0x4209f188: main_task at /opt/esp/idf/components/freertos/app_startup.c:208
--- 0x4037db31: vPortTaskWrapper at /opt/esp/idf/components/freertos/FreeRTOS-Kernel/portable/xtensa/port.c:139




ELF file SHA256: 7b441f547

Rebooting...
ESP-ROM:esp32s3-20210327
Build:Mar 27 2021
rst:0xc (RTC_SW_CPU_RST),boot:0x28 (SPI_FAST_FLASH_BOOT)
Saved PC:0x4037cfc9
--- 0x4037cfc9: esp_restart_noos at /opt/esp/idf/components/esp_system/port/soc/esp32s3/system_internal.c:164
SPIWP:0xee
mode:DIO, clock div:1
load:0x3fce2820,len:0x1588
load:0x403c8700,len:0xd20
load:0x403cb700,len:0x2f2c
entry 0x403c8924
I (24) boot: ESP-IDF v6.0-dev-1489-g4e036983a7 2nd stage bootloader
I (25) boot: compile time Aug 19 2025 21:57:35      
I (25) boot: Multicore bootloader
I (26) boot: chip revision: v0.2
I (29) boot: efuse block revision: v1.3
I (32) boot.esp32s3: Boot SPI Speed : 80MHz
I (36) boot.esp32s3: SPI Mode       : DIO
I (40) boot.esp32s3: SPI Flash Size : 2MB
I (44) boot: Enabling RNG early entropy source...   
I (48) boot: Partition Table:
I (51) boot: ## Label            Usage          Type ST Offset   Length
I (57) boot:  0 nvs              WiFi data        01 02 00009000 00006000
I (64) boot:  1 phy_init         RF data          01 01 0000f000 00001000
I (70) boot:  2 factory          factory app      00 00 00010000 00100000
I (77) boot: End of partition table
I (80) esp_image: segment 0: paddr=00010020 vaddr=3c0b0020 size=20b58h (133976) map
I (111) esp_image: segment 1: paddr=00030b80 vaddr=3fc9b700 size=04cb0h ( 19632) load
I (116) esp_image: segment 2: paddr=00035838 vaddr=40374000 size=0a7e0h ( 42976) load
I (126) esp_image: segment 3: paddr=00040020 vaddr=42000020 size=a01e0h (655840) map
I (242) esp_image: segment 4: paddr=000e0208 vaddr=4037e7e0 size=0cef4h ( 52980) load
I (254) esp_image: segment 5: paddr=000ed104 vaddr=50000000 size=00020h (    32) load
I (264) boot: Loaded app from partition at offset 0x10000
I (264) boot: Disabling RNG early entropy source... 
I (274) cpu_start: Multicore app
I (283) cpu_start: GPIO 44 and 43 are used as console UART I/O pins
I (284) cpu_start: Pro cpu start user code
I (284) cpu_start: cpu freq: 160000000 Hz
I (285) app_init: Application information:
I (289) app_init: Project name:     dshot_esc       
I (293) app_init: App version:      1
I (296) app_init: Compile time:     Aug 19 2025 21:57:23
I (301) app_init: ELF file SHA256:  7b441f547...    
I (306) app_init: ESP-IDF:          v6.0-dev-1489-g4e036983a7
I (311) efuse_init: Min chip rev:     v0.0
I (315) efuse_init: Max chip rev:     v0.99 
I (319) efuse_init: Chip rev:         v0.2
I (323) heap_init: Initializing. RAM available for dynamic allocation:
I (329) heap_init: At 3FCA4738 len 00044FD8 (275 KiB): RAM
I (334) heap_init: At 3FCE9710 len 00005724 (21 KiB): RAM
I (339) heap_init: At 3FCF0000 len 00008000 (32 KiB): DRAM
I (345) heap_init: At 600FE000 len 00001FE8 (7 KiB): RTCRAM
I (351) spi_flash: detected chip: generic
I (354) spi_flash: flash io: dio
W (356) spi_flash: Detected size(4096k) larger than the size in the binary image header(2048k). Using the size in the binary image header.
I (369) sleep_gpio: Configure to isolate all GPIO pins in sleep state
I (375) sleep_gpio: Enable automatic switching of GPIO sleep configuration
I (382) main_task: Started on CPU0
I (392) main_task: Calling app_main()
I (392) greenhouse: Init NVS
I (412) greenhouse: Init Wi‑Fi
I (412) pp: pp rom version: e7ae62f
I (412) net80211: net80211 rom version: e7ae62f     
I (422) wifi:wifi driver task: 3fced480, prio:23, stack:6656, core=0
I (432) wifi:wifi firmware version: 5984c42
I (432) wifi:wifi certification version: v7.0       
I (432) wifi:config NVS flash: enabled
I (432) wifi:config nano formatting: disabled       
I (432) wifi:Init data frame dynamic rx buffer num: 32
I (442) wifi:Init static rx mgmt buffer num: 5
I (442) wifi:Init management short buffer num: 32   
I (442) wifi:Init dynamic tx buffer num: 32
I (452) wifi:Init static tx FG buffer num: 2        
I (452) wifi:Init static rx buffer size: 1600
I (462) wifi:Init static rx buffer num: 10
I (462) wifi:Init dynamic rx buffer num: 32
I (472) wifi_init: rx ba win: 6
I (472) wifi_init: accept mbox: 6
I (472) wifi_init: tcpip mbox: 32
I (472) wifi_init: udp mbox: 6
I (482) wifi_init: tcp mbox: 6
I (482) wifi_init: tcp tx win: 5760
I (482) wifi_init: tcp rx win: 5760
I (492) wifi_init: tcp mss: 1440
I (492) wifi_init: WiFi IRAM OP enabled
I (492) wifi_init: WiFi RX IRAM OP enabled
I (502) phy_init: phy_version 701,f4f1da3a,Mar  3 2025,15:50:10
I (542) wifi:mode : sta (f0:f5:bd:79:1b:a0)
I (542) wifi:enable tsf
I (542) greenhouse: wifi_init_sta finished, waiting for IP...
I (542) greenhouse: Wi‑Fi start, connecting...      

I (552) wifi:new:<10,0>, old:<1,0>, ap:<255,255>, sta:<10,0>, prof:1, snd_ch_cfg:0x0
I (562) wifi:state: init -> auth (0xb0)
I (562) wifi:state: auth -> assoc (0x0)
I (572) wifi:state: assoc -> run (0x10)
I (632) wifi:connected with Tent, aid = 38, channel 10, BW20, bssid = 82:9c:25:89:44:45
I (632) wifi:security: WPA2-PSK, phy: bgn, rssi: -34
I (642) wifi:pm start, type: 1

I (642) wifi:dp: 1, bi: 102400, li: 3, scale listen interval from 307200 us to 307200 us
I (642) wifi:set rx beacon pti, rx_bcn_pti: 0, bcn_timeout: 25000, mt_pti: 0, mt_time: 10000
I (652) greenhouse: Wi‑Fi connected to AP
I (662) wifi:<ba-add>idx:0 (ifx:0, 82:9c:25:89:44:45), tid:7, ssn:3, winSize:64
I (722) wifi:AP's beacon interval = 102400 us, DTIM period = 1
I (1672) esp_netif_handlers: sta ip: 192.168.53.147, mask: 255.255.255.0, gw: 192.168.53.1
I (1672) greenhouse: Got IP: 192.168.53.147, GW: 192.168.53.1, Netmask: 255.255.255.0
I (1672) greenhouse: Init MQTT
I (1682) greenhouse: Init DShot motors
E (1682) rmt: rmt_tx_enable(758): channel not in init state
ESP_ERROR_CHECK failed: esp_err_t 0x103 (ESP_ERR_INVALID_STATE) at 0x4200c190
--- 0x4200c190: motor_apply at /workspaces/dshot_esc/main/app_main.c:107
file: "./main/app_main.c" line 107
func: motor_apply
expression: rmt_enable(m->chan)

abort() was called at PC 0x4037ce37 on core 0       
--- 0x4037ce37: _esp_error_check_failed at /opt/esp/idf/components/esp_system/esp_err.c:49


Backtrace: 0x4037ce79:0x3fca80f0 0x4037ce41:0x3fca8110 0x40385139:0x3fca8130 0x4037ce37:0x3fca81a0 0x4200c190:0x3fca81d0 0x4200c65f:0x3fca8200 0x4200c737:0x3fca8250 0x4209f188:0x3fca8290 0x4037db31:0x3fca82c0
--- 0x4037ce79: panic_abort at /opt/esp/idf/components/esp_system/panic.c:469
--- 0x4037ce41: esp_system_abort at /opt/esp/idf/components/esp_system/port/esp_system_chip.c:87        
--- 0x40385139: abort at /opt/esp/idf/components/newlib/src/abort.c:38
--- 0x4037ce37: _esp_error_check_failed at /opt/esp/idf/components/esp_system/esp_err.c:49
--- 0x4200c190: motor_apply at /workspaces/dshot_esc/main/app_main.c:107
--- 0x4200c65f: dshot_init_motor at /workspaces/dshot_esc/main/app_main.c:446
--- 0x4200c737: app_main at /workspaces/dshot_esc/main/app_main.c:469
--- 0x4209f188: main_task at /opt/esp/idf/components/freertos/app_startup.c:208
--- 0x4037db31: vPortTaskWrapper at /opt/esp/idf/components/freertos/FreeRTOS-Kernel/portable/xtensa/port.c:139




ELF file SHA256: 7b441f547

Rebooting...
ESP-ROM:esp32s3-20210327
Build:Mar 27 2021
rst:0xc (RTC_SW_CPU_RST),boot:0x28 (SPI_FAST_FLASH_BOOT)
Saved PC:0x4037cfc9
--- 0x4037cfc9: esp_restart_noos at /opt/esp/idf/components/esp_system/port/soc/esp32s3/system_internal.c:164
SPIWP:0xee
mode:DIO, clock div:1
load:0x3fce2820,len:0x1588
load:0x403c8700,len:0xd20
load:0x403cb700,len:0x2f2c
entry 0x403c8924
I (24) boot: ESP-IDF v6.0-dev-1489-g4e036983a7 2nd stage bootloader
I (25) boot: compile time Aug 19 2025 21:57:35      
I (25) boot: Multicore bootloader
I (26) boot: chip revision: v0.2
I (29) boot: efuse block revision: v1.3
I (33) boot.esp32s3: Boot SPI Speed : 80MHz
I (36) boot.esp32s3: SPI Mode       : DIO
I (40) boot.esp32s3: SPI Flash Size : 2MB
I (44) boot: Enabling RNG early entropy source...   
I (48) boot: Partition Table:
I (51) boot: ## Label            Usage          Type ST Offset   Length
I (57) boot:  0 nvs              WiFi data        01 02 00009000 00006000
I (64) boot:  1 phy_init         RF data          01 01 0000f000 00001000
I (70) boot:  2 factory          factory app      00 00 00010000 00100000
I (77) boot: End of partition table
I (80) esp_image: segment 0: paddr=00010020 vaddr=3c0b0020 size=20b58h (133976) map
I (111) esp_image: segment 1: paddr=00030b80 vaddr=3fc9b700 size=04cb0h ( 19632) load
I (116) esp_image: segment 2: paddr=00035838 vaddr=40374000 size=0a7e0h ( 42976) load
I (126) esp_image: segment 3: paddr=00040020 vaddr=42000020 size=a01e0h (655840) map
I (242) esp_image: segment 4: paddr=000e0208 vaddr=4037e7e0 size=0cef4h ( 52980) load
I (254) esp_image: segment 5: paddr=000ed104 vaddr=50000000 size=00020h (    32) load
I (264) boot: Loaded app from partition at offset 0x10000
I (264) boot: Disabling RNG early entropy source... 
I (274) cpu_start: Multicore app
I (283) cpu_start: GPIO 44 and 43 are used as console UART I/O pins
I (284) cpu_start: Pro cpu start user code
I (284) cpu_start: cpu freq: 160000000 Hz
I (285) app_init: Application information:
I (289) app_init: Project name:     dshot_esc       
I (293) app_init: App version:      1
I (296) app_init: Compile time:     Aug 19 2025 21:57:23
I (301) app_init: ELF file SHA256:  7b441f547...    
I (306) app_init: ESP-IDF:          v6.0-dev-1489-g4e036983a7
I (311) efuse_init: Min chip rev:     v0.0
I (315) efuse_init: Max chip rev:     v0.99
I (319) efuse_init: Chip rev:         v0.2
I (323) heap_init: Initializing. RAM available for dynamic allocation:
I (329) heap_init: At 3FCA4738 len 00044FD8 (275 KiB): RAM
I (334) heap_init: At 3FCE9710 len 00005724 (21 KiB): RAM
I (340) heap_init: At 3FCF0000 len 00008000 (32 KiB): DRAM
I (345) heap_init: At 600FE000 len 00001FE8 (7 KiB): RTCRAM
I (351) spi_flash: detected chip: generic
I (354) spi_flash: flash io: dio
W (357) spi_flash: Detected size(4096k) larger than the size in the binary image header(2048k). Using the size in the binary image header.
I (369) sleep_gpio: Configure to isolate all GPIO pins in sleep state
I (375) sleep_gpio: Enable automatic switching of GPIO sleep configuration
I (382) main_task: Started on CPU0
I (392) main_task: Calling app_main()
I (392) greenhouse: Init NVS
I (412) greenhouse: Init Wi‑Fi
I (412) pp: pp rom version: e7ae62f
I (412) net80211: net80211 rom version: e7ae62f     
I (422) wifi:wifi driver task: 3fced480, prio:23, stack:6656, core=0
I (432) wifi:wifi firmware version: 5984c42
I (432) wifi:wifi certification version: v7.0       
I (432) wifi:config NVS flash: enabled
I (432) wifi:config nano formatting: disabled       
I (432) wifi:Init data frame dynamic rx buffer num: 32
I (442) wifi:Init static rx mgmt buffer num: 5      
I (442) wifi:Init management short buffer num: 32   
I (442) wifi:Init dynamic tx buffer num: 32
I (452) wifi:Init static tx FG buffer num: 2
I (452) wifi:Init static rx buffer size: 1600       
I (462) wifi:Init static rx buffer num: 10
I (462) wifi:Init dynamic rx buffer num: 32
I (472) wifi_init: rx ba win: 6
I (472) wifi_init: accept mbox: 6
I (472) wifi_init: tcpip mbox: 32
I (472) wifi_init: udp mbox: 6
I (482) wifi_init: tcp mbox: 6
I (482) wifi_init: tcp tx win: 5760
I (482) wifi_init: tcp rx win: 5760
I (492) wifi_init: tcp mss: 1440
I (492) wifi_init: WiFi IRAM OP enabled
I (492) wifi_init: WiFi RX IRAM OP enabled
I (502) phy_init: phy_version 701,f4f1da3a,Mar  3 2025,15:50:10
I (552) wifi:mode : sta (f0:f5:bd:79:1b:a0)
I (552) wifi:enable tsf
I (552) greenhouse: wifi_init_sta finished, waiting for IP...
I (552) greenhouse: Wi‑Fi start, connecting...      

I (562) wifi:new:<10,0>, old:<1,0>, ap:<255,255>, sta:<10,0>, prof:1, snd_ch_cfg:0x0
I (562) wifi:state: init -> auth (0xb0)
I (572) wifi:state: auth -> assoc (0x0)
I (572) wifi:state: assoc -> run (0x10)
I (662) wifi:connected with Tent, aid = 20, channel 10, BW20, bssid = 82:9c:25:89:44:45
I (662) wifi:security: WPA2-PSK, phy: bgn, rssi: -34
I (662) wifi:pm start, type: 1

I (672) wifi:dp: 1, bi: 102400, li: 3, scale listen interval from 307200 us to 307200 us
I (672) wifi:set rx beacon pti, rx_bcn_pti: 0, bcn_timeout: 25000, mt_pti: 0, mt_time: 10000
I (682) greenhouse: Wi‑Fi connected to AP
I (692) wifi:<ba-add>idx:0 (ifx:0, 82:9c:25:89:44:45), tid:7, ssn:3, winSize:64
I (722) wifi:AP's beacon interval = 102400 us, DTIM period = 1
I (1702) esp_netif_handlers: sta ip: 192.168.53.147, mask: 255.255.255.0, gw: 192.168.53.1
I (1702) greenhouse: Got IP: 192.168.53.147, GW: 192.168.53.1, Netmask: 255.255.255.0
I (1702) greenhouse: Init MQTT
I (1712) greenhouse: Init DShot motors
E (1712) rmt: rmt_tx_enable(758): channel not in init state
ESP_ERROR_CHECK failed: esp_err_t 0x103 (ESP_ERR_INVALID_STATE) at 0x4200c190
--- 0x4200c190: motor_apply at /workspaces/dshot_esc/main/app_main.c:107
file: "./main/app_main.c" line 107
func: motor_apply
expression: rmt_enable(m->chan)

abort() was called at PC 0x4037ce37 on core 0       
--- 0x4037ce37: _esp_error_check_failed at /opt/esp/idf/components/esp_system/esp_err.c:49


Backtrace: 0x4037ce79:0x3fca80f0 0x4037ce41:0x3fca8110 0x40385139:0x3fca8130 0x4037ce37:0x3fca81a0 0x4200c190:0x3fca81d0 0x4200c65f:0x3fca8200 0x4200c737:0x3fca8250 0x4209f188:0x3fca8290 0x4037db31:0x3fca82c0
--- 0x4037ce79: panic_abort at /opt/esp/idf/components/esp_system/panic.c:469
--- 0x4037ce41: esp_system_abort at /opt/esp/idf/components/esp_system/port/esp_system_chip.c:87        
--- 0x40385139: abort at /opt/esp/idf/components/newlib/src/abort.c:38
--- 0x4037ce37: _esp_error_check_failed at /opt/esp/idf/components/esp_system/esp_err.c:49
--- 0x4200c190: motor_apply at /workspaces/dshot_esc/main/app_main.c:107
--- 0x4200c65f: dshot_init_motor at /workspaces/dshot_esc/main/app_main.c:446
--- 0x4200c737: app_main at /workspaces/dshot_esc/main/app_main.c:469
--- 0x4209f188: main_task at /opt/esp/idf/components/freertos/app_startup.c:208
--- 0x4037db31: vPortTaskWrapper at /opt/esp/idf/components/freertos/FreeRTOS-Kernel/portable/xtensa/port.c:139




ELF file SHA256: 7b441f547

Rebooting...
ESP-ROM:esp32s3-20210327
Build:Mar 27 2021
rst:0xc (RTC_SW_CPU_RST),boot:0x28 (SPI_FAST_FLASH_BOOT)
Saved PC:0x4037cfc9
--- 0x4037cfc9: esp_restart_noos at /opt/esp/idf/components/esp_system/port/soc/esp32s3/system_internal.c:164
SPIWP:0xee
mode:DIO, clock div:1
load:0x3fce2820,len:0x1588
load:0x403c8700,len:0xd20
load:0x403cb700,len:0x2f2c
entry 0x403c8924
I (24) boot: ESP-IDF v6.0-dev-1489-g4e036983a7 2nd stage bootloader
I (25) boot: compile time Aug 19 2025 21:57:35      
I (25) boot: Multicore bootloader
I (26) boot: chip revision: v0.2
I (29) boot: efuse block revision: v1.3
I (32) boot.esp32s3: Boot SPI Speed : 80MHz
I (36) boot.esp32s3: SPI Mode       : DIO
I (40) boot.esp32s3: SPI Flash Size : 2MB
I (44) boot: Enabling RNG early entropy source...   
I (48) boot: Partition Table:
I (51) boot: ## Label            Usage          Type ST Offset   Length
I (57) boot:  0 nvs              WiFi data        01 02 00009000 00006000
I (64) boot:  1 phy_init         RF data          01 01 0000f000 00001000
I (70) boot:  2 factory          factory app      00 00 00010000 00100000
I (77) boot: End of partition table
I (80) esp_image: segment 0: paddr=00010020 vaddr=3c0b0020 size=20b58h (133976) map
I (111) esp_image: segment 1: paddr=00030b80 vaddr=3fc9b700 size=04cb0h ( 19632) load
I (116) esp_image: segment 2: paddr=00035838 vaddr=40374000 size=0a7e0h ( 42976) load
I (126) esp_image: segment 3: paddr=00040020 vaddr=42000020 size=a01e0h (655840) map
I (242) esp_image: segment 4: paddr=000e0208 vaddr=4037e7e0 size=0cef4h ( 52980) load
I (254) esp_image: segment 5: paddr=000ed104 vaddr=50000000 size=00020h (    32) load
I (265) boot: Loaded app from partition at offset 0x10000
I (265) boot: Disabling RNG early entropy source... 
I (275) cpu_start: Multicore app
I (283) cpu_start: GPIO 44 and 43 are used as console UART I/O pins
I (284) cpu_start: Pro cpu start user code
I (284) cpu_start: cpu freq: 160000000 Hz
I (286) app_init: Application information:
I (289) app_init: Project name:     dshot_esc       
I (293) app_init: App version:      1
I (297) app_init: Compile time:     Aug 19 2025 21:57:23
I (302) app_init: ELF file SHA256:  7b441f547...    
I (306) app_init: ESP-IDF:          v6.0-dev-1489-g4e036983a7
I (312) efuse_init: Min chip rev:     v0.0
I (316) efuse_init: Max chip rev:     v0.99
I (320) efuse_init: Chip rev:         v0.2
I (323) heap_init: Initializing. RAM available for dynamic allocation:
I (330) heap_init: At 3FCA4738 len 00044FD8 (275 KiB): RAM
I (335) heap_init: At 3FCE9710 len 00005724 (21 KiB): RAM
I (340) heap_init: At 3FCF0000 len 00008000 (32 KiB): DRAM
I (345) heap_init: At 600FE000 len 00001FE8 (7 KiB): RTCRAM
I (352) spi_flash: detected chip: generic
I (354) spi_flash: flash io: dio
W (357) spi_flash: Detected size(4096k) larger than the size in the binary image header(2048k). Using the size in the binary image header.
I (370) sleep_gpio: Configure to isolate all GPIO pins in sleep state
I (376) sleep_gpio: Enable automatic switching of GPIO sleep configuration
I (383) main_task: Started on CPU0
I (393) main_task: Calling app_main()
I (393) greenhouse: Init NVS
I (413) greenhouse: Init Wi‑Fi
I (413) pp: pp rom version: e7ae62f
I (413) net80211: net80211 rom version: e7ae62f     
I (423) wifi:wifi driver task: 3fced480, prio:23, stack:6656, core=0
I (433) wifi:wifi firmware version: 5984c42
I (433) wifi:wifi certification version: v7.0       
I (433) wifi:config NVS flash: enabled
I (433) wifi:config nano formatting: disabled       
I (433) wifi:Init data frame dynamic rx buffer num: 32
I (443) wifi:Init static rx mgmt buffer num: 5      
I (443) wifi:Init management short buffer num: 32   
I (443) wifi:Init dynamic tx buffer num: 32
I (453) wifi:Init static tx FG buffer num: 2
I (453) wifi:Init static rx buffer size: 1600       
I (463) wifi:Init static rx buffer num: 10
I (463) wifi:Init dynamic rx buffer num: 32
I (473) wifi_init: rx ba win: 6
I (473) wifi_init: accept mbox: 6
I (473) wifi_init: tcpip mbox: 32
I (473) wifi_init: udp mbox: 6
I (483) wifi_init: tcp mbox: 6
I (483) wifi_init: tcp tx win: 5760
I (483) wifi_init: tcp rx win: 5760
I (493) wifi_init: tcp mss: 1440
I (493) wifi_init: WiFi IRAM OP enabled
I (493) wifi_init: WiFi RX IRAM OP enabled
I (503) phy_init: phy_version 701,f4f1da3a,Mar  3 2025,15:50:10
I (543) wifi:mode : sta (f0:f5:bd:79:1b:a0)
I (543) wifi:enable tsf
I (543) greenhouse: wifi_init_sta finished, waiting for IP...
I (543) greenhouse: Wi‑Fi start, connecting...      

I (553) wifi:new:<10,0>, old:<1,0>, ap:<255,255>, sta:<10,0>, prof:1, snd_ch_cfg:0x0
I (553) wifi:state: init -> auth (0xb0)
I (563) wifi:state: auth -> assoc (0x0)
I (573) wifi:state: assoc -> run (0x10)
I (613) wifi:connected with Tent, aid = 24, channel 10, BW20, bssid = 82:9c:25:89:44:45
I (613) wifi:security: WPA2-PSK, phy: bgn, rssi: -34
I (613) wifi:pm start, type: 1

I (613) wifi:dp: 1, bi: 102400, li: 3, scale listen interval from 307200 us to 307200 us
I (623) wifi:set rx beacon pti, rx_bcn_pti: 0, bcn_timeout: 25000, mt_pti: 0, mt_time: 10000
I (633) greenhouse: Wi‑Fi connected to AP
I (633) wifi:<ba-add>idx:0 (ifx:0, 82:9c:25:89:44:45), tid:7, ssn:3, winSize:64
I (693) wifi:AP's beacon interval = 102400 us, DTIM period = 1
I (1643) esp_netif_handlers: sta ip: 192.168.53.147, mask: 255.255.255.0, gw: 192.168.53.1
I (1643) greenhouse: Got IP: 192.168.53.147, GW: 192.168.53.1, Netmask: 255.255.255.0
I (1643) greenhouse: Init MQTT
I (1653) greenhouse: Init DShot motors
E (1653) rmt: rmt_tx_enable(758): channel not in init state
ESP_ERROR_CHECK failed: esp_err_t 0x103 (ESP_ERR_INVALID_STATE) at 0x4200c190
--- 0x4200c190: motor_apply at /workspaces/dshot_esc/main/app_main.c:107
file: "./main/app_main.c" line 107
func: motor_apply
expression: rmt_enable(m->chan)

abort() was called at PC 0x4037ce37 on core 0       
--- 0x4037ce37: _esp_error_check_failed at /opt/esp/idf/components/esp_system/esp_err.c:49


Backtrace: 0x4037ce79:0x3fca80f0 0x4037ce41:0x3fca8110 0x40385139:0x3fca8130 0x4037ce37:0x3fca81a0 0x4200c190:0x3fca81d0 0x4200c65f:0x3fca8200 0x4200c737:0x3fca8250 0x4209f188:0x3fca8290 0x4037db31:0x3fca82c0
--- 0x4037ce79: panic_abort at /opt/esp/idf/components/esp_system/panic.c:469
--- 0x4037ce41: esp_system_abort at /opt/esp/idf/components/esp_system/port/esp_system_chip.c:87        
--- 0x40385139: abort at /opt/esp/idf/components/newlib/src/abort.c:38
--- 0x4037ce37: _esp_error_check_failed at /opt/esp/idf/components/esp_system/esp_err.c:49
--- 0x4200c190: motor_apply at /workspaces/dshot_esc/main/app_main.c:107
--- 0x4200c65f: dshot_init_motor at /workspaces/dshot_esc/main/app_main.c:446
--- 0x4200c737: app_main at /workspaces/dshot_esc/main/app_main.c:469
--- 0x4209f188: main_task at /opt/esp/idf/components/freertos/app_startup.c:208
--- 0x4037db31: vPortTaskWrapper at /opt/esp/idf/components/freertos/FreeRTOS-Kernel/portable/xtensa/port.c:139




ELF file SHA256: 7b441f547

Rebooting...
ESP-ROM:esp32s3-20210327
Build:Mar 27 2021
rst:0xc (RTC_SW_CPU_RST),boot:0x28 (SPI_FAST_FLASH_BOOT)
Saved PC:0x4037cfc9
--- 0x4037cfc9: esp_restart_noos at /opt/esp/idf/components/esp_system/port/soc/esp32s3/system_internal.c:164
SPIWP:0xee
mode:DIO, clock div:1
load:0x3fce2820,len:0x1588
load:0x403c8700,len:0xd20
load:0x403cb700,len:0x2f2c
entry 0x403c8924
I (24) boot: ESP-IDF v6.0-dev-1489-g4e036983a7 2nd stage bootloader
I (25) boot: compile time Aug 19 2025 21:57:35      
I (25) boot: Multicore bootloader
I (26) boot: chip revision: v0.2
I (29) boot: efuse block revision: v1.3
I (32) boot.esp32s3: Boot SPI Speed : 80MHz
I (36) boot.esp32s3: SPI Mode       : DIO
I (40) boot.esp32s3: SPI Flash Size : 2MB
I (44) boot: Enabling RNG early entropy source...   
I (48) boot: Partition Table:
I (51) boot: ## Label            Usage          Type ST Offset   Length
I (57) boot:  0 nvs              WiFi data        01 02 00009000 00006000
I (64) boot:  1 phy_init         RF data          01 01 0000f000 00001000
I (70) boot:  2 factory          factory app      00 00 00010000 00100000
I (77) boot: End of partition table
I (80) esp_image: segment 0: paddr=00010020 vaddr=3c0b0020 size=20b58h (133976) map
I (111) esp_image: segment 1: paddr=00030b80 vaddr=3fc9b700 size=04cb0h ( 19632) load
I (116) esp_image: segment 2: paddr=00035838 vaddr=40374000 size=0a7e0h ( 42976) load
I (126) esp_image: segment 3: paddr=00040020 vaddr=42000020 size=a01e0h (655840) map
I (242) esp_image: segment 4: paddr=000e0208 vaddr=4037e7e0 size=0cef4h ( 52980) load
I (254) esp_image: segment 5: paddr=000ed104 vaddr=50000000 size=00020h (    32) load
I (265) boot: Loaded app from partition at offset 0x10000
I (265) boot: Disabling RNG early entropy source... 
I (275) cpu_start: Multicore app
I (283) cpu_start: GPIO 44 and 43 are used as console UART I/O pins
I (284) cpu_start: Pro cpu start user code
I (284) cpu_start: cpu freq: 160000000 Hz
I (286) app_init: Application information:
I (289) app_init: Project name:     dshot_esc       
I (293) app_init: App version:      1
I (297) app_init: Compile time:     Aug 19 2025 21:57:23
I (302) app_init: ELF file SHA256:  7b441f547...    
I (306) app_init: ESP-IDF:          v6.0-dev-1489-g4e036983a7
I (312) efuse_init: Min chip rev:     v0.0
I (316) efuse_init: Max chip rev:     v0.99
I (320) efuse_init: Chip rev:         v0.2
I (323) heap_init: Initializing. RAM available for dynamic allocation:
I (330) heap_init: At 3FCA4738 len 00044FD8 (275 KiB): RAM
I (335) heap_init: At 3FCE9710 len 00005724 (21 KiB): RAM
I (340) heap_init: At 3FCF0000 len 00008000 (32 KiB): DRAM
I (345) heap_init: At 600FE000 len 00001FE8 (7 KiB): RTCRAM
I (352) spi_flash: detected chip: generic
I (354) spi_flash: flash io: dio
W (357) spi_flash: Detected size(4096k) larger than the size in the binary image header(2048k). Using the size in the binary image header.
I (370) sleep_gpio: Configure to isolate all GPIO pins in sleep state
I (376) sleep_gpio: Enable automatic switching of GPIO sleep configuration
I (383) main_task: Started on CPU0
I (393) main_task: Calling app_main()
I (393) greenhouse: Init NVS
I (413) greenhouse: Init Wi‑Fi
I (413) pp: pp rom version: e7ae62f
I (413) net80211: net80211 rom version: e7ae62f     
I (423) wifi:wifi driver task: 3fced480, prio:23, stack:6656, core=0
I (433) wifi:wifi firmware version: 5984c42
I (433) wifi:wifi certification version: v7.0       
I (433) wifi:config NVS flash: enabled
I (433) wifi:config nano formatting: disabled
I (433) wifi:Init data frame dynamic rx buffer num: 32
I (443) wifi:Init static rx mgmt buffer num: 5      
I (443) wifi:Init management short buffer num: 32   
I (443) wifi:Init dynamic tx buffer num: 32
I (453) wifi:Init static tx FG buffer num: 2        
I (453) wifi:Init static rx buffer size: 1600       
I (463) wifi:Init static rx buffer num: 10
I (463) wifi:Init dynamic rx buffer num: 32
I (473) wifi_init: rx ba win: 6
I (473) wifi_init: accept mbox: 6
I (473) wifi_init: tcpip mbox: 32
I (473) wifi_init: udp mbox: 6
I (483) wifi_init: tcp mbox: 6
I (483) wifi_init: tcp tx win: 5760
I (483) wifi_init: tcp rx win: 5760
I (493) wifi_init: tcp mss: 1440
I (493) wifi_init: WiFi IRAM OP enabled
I (493) wifi_init: WiFi RX IRAM OP enabled
I (503) phy_init: phy_version 701,f4f1da3a,Mar  3 2025,15:50:10
I (533) phy_init: Saving new calibration data due to checksum failure or outdated calibration data, mode(0)
I (593) wifi:mode : sta (f0:f5:bd:79:1b:a0)
I (593) wifi:enable tsf
I (593) greenhouse: wifi_init_sta finished, waiting for IP...
I (603) greenhouse: Wi‑Fi start, connecting...      

I (613) wifi:new:<10,0>, old:<1,0>, ap:<255,255>, sta:<10,0>, prof:1, snd_ch_cfg:0x0
I (613) wifi:state: init -> auth (0xb0)
I (633) wifi:state: auth -> assoc (0x0)
I (643) wifi:state: assoc -> run (0x10)
I (683) wifi:connected with Tent, aid = 10, channel 10, BW20, bssid = 82:9c:25:89:44:45
I (683) wifi:security: WPA2-PSK, phy: bgn, rssi: -61
I (683) wifi:pm start, type: 1

I (683) wifi:dp: 1, bi: 102400, li: 3, scale listen interval from 307200 us to 307200 us
I (693) wifi:set rx beacon pti, rx_bcn_pti: 0, bcn_timeout: 25000, mt_pti: 0, mt_time: 10000
I (703) greenhouse: Wi‑Fi connected to AP
I (733) wifi:AP's beacon interval = 102400 us, DTIM period = 1
I (733) wifi:<ba-add>idx:0 (ifx:0, 82:9c:25:89:44:45), tid:7, ssn:3, winSize:64