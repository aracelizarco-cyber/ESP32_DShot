PS C:\Users\Carlo\workspace\dshot_esc> $env:IDF_PATH = 'C:\Users\Carlo\v5.5\esp-idf';
PS C:\Users\Carlo\workspace\dshot_esc>  & 'C:\Users\Carlo\.espressif\python_env\idf5.5_py3.11_env\Scripts\python.exe' 'C:\Users\Carlo\v5.5\esp-idf\tools\idf_monitor.py' -p COM6 -b 115200 --toolchain-prefix xtensa-esp32s3-elf- --make '''C:\Users\Carlo\.espressif\python_env\idf5.5_py3.11_env\Scripts\python.exe'' ''C:\Users\Carlo\v5.5\esp-idf\tools\idf.py''' --target esp32s3 'c:\Users\Carlo\workspace\dshot_esc\build\dshot_esc.elf'
--- Warning: GDB cannot open serial ports accessed as COMx
--- Using \\.\COM6 instead...
--- esp-idf-monitor 1.7.0 on \\.\COM6 115200
--- Quit: Ctrl+] | Menu: Ctrl+T | Help: Ctrl+T followed by Ctrl+H
ESP-ROM:esp32s3-20210327
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
I (25) boot: compile time Aug 21 2025 01:39:41
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
I (81) esp_image: segment 0: paddr=00010020 vaddr=3c0a0020 size=20574h (132468) map
I (111) esp_image: segment 1: paddr=0003059c vaddr=3fc9b700 size=04cb0h ( 19632) load
I (116) esp_image: segment 2: paddr=00035254 vaddr=40374000 size=0adc4h ( 44484) load
I (126) esp_image: segment 3: paddr=00040020 vaddr=42000020 size=9f7d4h (653268) map
I (242) esp_image: segment 4: paddr=000df7fc vaddr=4037edc4 size=0c910h ( 51472) load
I (254) esp_image: segment 5: paddr=000ec114 vaddr=50000000 size=00020h (    32) load
I (265) boot: Loaded app from partition at offset 0x10000
I (265) boot: Disabling RNG early entropy source...
I (275) cpu_start: Multicore app
I (283) cpu_start: GPIO 44 and 43 are used as console UART I/O pins
I (284) cpu_start: Pro cpu start user code
I (284) cpu_start: cpu freq: 160000000 Hz
I (286) app_init: Application information:
I (289) app_init: Project name:     dshot_esc
I (293) app_init: App version:      1
I (297) app_init: Compile time:     Aug 21 2025 01:39:31
I (302) app_init: ELF file SHA256:  618177a30...
I (306) app_init: ESP-IDF:          v6.0-dev-1489-g4e036983a7
I (312) efuse_init: Min chip rev:     v0.0
I (315) efuse_init: Max chip rev:     v0.99
I (319) efuse_init: Chip rev:         v0.2
I (323) heap_init: Initializing. RAM available for dynamic allocation:        
I (330) heap_init: At 3FCA4890 len 00044E80 (275 KiB): RAM
I (335) heap_init: At 3FCE9710 len 00005724 (21 KiB): RAM
I (340) heap_init: At 3FCF0000 len 00008000 (32 KiB): DRAM
I (345) heap_init: At 600FE000 len 00001FE8 (7 KiB): RTCRAM
I (351) spi_flash: detected chip: generic
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
I (423) wifi:wifi driver task: 3fced504, prio:23, stack:6656, core=0
I (483) wifi:wifi firmware version: 5984c42
I (483) wifi:wifi certification version: v7.0
I (483) wifi:config NVS flash: enabled
I (483) wifi:config nano formatting: disabled
I (493) wifi:Init data frame dynamic rx buffer num: 32
I (493) wifi:Init static rx mgmt buffer num: 5
I (503) wifi:Init management short buffer num: 32
I (503) wifi:Init dynamic tx buffer num: 32
I (513) wifi:Init static tx FG buffer num: 2
I (513) wifi:Init static rx buffer size: 1600
I (513) wifi:Init static rx buffer num: 10
I (523) wifi:Init dynamic rx buffer num: 32
I (523) wifi_init: rx ba win: 6
I (523) wifi_init: accept mbox: 6
I (533) wifi_init: tcpip mbox: 32
I (533) wifi_init: udp mbox: 6
I (533) wifi_init: tcp mbox: 6
I (543) wifi_init: tcp tx win: 5760
I (543) wifi_init: tcp rx win: 5760
I (543) wifi_init: tcp mss: 1440
I (543) wifi_init: WiFi IRAM OP enabled
I (553) wifi_init: WiFi RX IRAM OP enabled
I (553) phy_init: phy_version 701,f4f1da3a,Mar  3 2025,15:50:10
I (603) wifi:mode : sta (f0:f5:bd:79:1b:a0)
I (603) wifi:enable tsf
I (603) wifi:Set ps type: 0, coexist: 0

I (603) greenhouse: Wi‑Fi start, connecting...

I (603) greenhouse: wifi_init_sta finished, waiting for IP...
I (643) wifi:new:<10,0>, old:<1,0>, ap:<255,255>, sta:<10,0>, prof:1, snd_ch_cfg:0x0
I (643) wifi:state: init -> auth (0xb0)
I (1643) wifi:state: auth -> init (0x200)
I (1653) wifi:new:<10,0>, old:<10,0>, ap:<255,255>, sta:<10,0>, prof:1, snd_ch_cfg:0x0
W (1653) greenhouse: Wi‑Fi disconnected, retrying...
W (2983) greenhouse: Wi‑Fi disconnected, retrying...
I (2993) wifi:new:<10,0>, old:<10,0>, ap:<255,255>, sta:<10,0>, prof:1, snd_ch_cfg:0x0
I (3003) wifi:state: init -> auth (0xb0)
I (4013) wifi:state: auth -> init (0x200)
I (4023) wifi:new:<10,0>, old:<10,0>, ap:<255,255>, sta:<10,0>, prof:1, snd_ch_cfg:0x0
W (4023) greenhouse: Wi‑Fi disconnected, retrying...
W (5353) greenhouse: Wi‑Fi disconnected, retrying...
I (5453) wifi:new:<10,0>, old:<10,0>, ap:<255,255>, sta:<10,0>, prof:1, snd_ch_cfg:0x0
I (5453) wifi:state: init -> auth (0xb0)
I (6463) wifi:state: auth -> init (0x200)
I (6473) wifi:new:<10,0>, old:<10,0>, ap:<255,255>, sta:<10,0>, prof:1, snd_ch_cfg:0x0
W (6473) greenhouse: Wi‑Fi disconnected, retrying...
W (7803) greenhouse: Wi‑Fi disconnected, retrying...
I (7813) wifi:new:<10,0>, old:<10,0>, ap:<255,255>, sta:<10,0>, prof:1, snd_ch_cfg:0x0
I (7813) wifi:state: init -> auth (0xb0)
I (8823) wifi:state: auth -> init (0x200)
I (8833) wifi:new:<10,0>, old:<10,0>, ap:<255,255>, sta:<10,0>, prof:1, snd_ch_cfg:0x0
W (8833) greenhouse: Wi‑Fi disconnected, retrying...
W (10163) greenhouse: Wi‑Fi disconnected, retrying...
I (10263) wifi:new:<10,0>, old:<10,0>, ap:<255,255>, sta:<10,0>, prof:1, snd_ch_cfg:0x0
I (10263) wifi:state: init -> auth (0xb0)
I (11283) wifi:state: auth -> init (0x200)
I (11293) wifi:new:<10,0>, old:<10,0>, ap:<255,255>, sta:<10,0>, prof:1, snd_ch_cfg:0x0
W (11293) greenhouse: Wi‑Fi disconnected, retrying...
W (12623) greenhouse: Wi‑Fi disconnected, retrying...
I (12723) wifi:new:<10,0>, old:<10,0>, ap:<255,255>, sta:<10,0>, prof:1, snd_ch_cfg:0x0
I (12723) wifi:state: init -> auth (0xb0)
I (13733) wifi:state: auth -> init (0x200)
I (13743) wifi:new:<10,0>, old:<10,0>, ap:<255,255>, sta:<10,0>, prof:1, snd_ch_cfg:0x0
W (13743) greenhouse: Wi‑Fi disconnected, retrying...
W (15073) greenhouse: Wi‑Fi disconnected, retrying...
I (15083) wifi:new:<10,0>, old:<10,0>, ap:<255,255>, sta:<10,0>, prof:1, snd_ch_cfg:0x0
I (15083) wifi:state: init -> auth (0xb0)
I (16093) wifi:state: auth -> init (0x200)
I (16103) wifi:new:<10,0>, old:<10,0>, ap:<255,255>, sta:<10,0>, prof:1, snd_ch_cfg:0x0
W (16103) greenhouse: Wi‑Fi disconnected, retrying...
W (17433) greenhouse: Wi‑Fi disconnected, retrying...
I (17533) wifi:new:<10,0>, old:<10,0>, ap:<255,255>, sta:<10,0>, prof:1, snd_ch_cfg:0x0
I (17543) wifi:state: init -> auth (0xb0)
I (18553) wifi:state: auth -> init (0x200)
I (18563) wifi:new:<10,0>, old:<10,0>, ap:<255,255>, sta:<10,0>, prof:1, snd_ch_cfg:0x0
W (18563) greenhouse: Wi‑Fi disconnected, retrying...
W (19893) greenhouse: Wi‑Fi disconnected, retrying...
I (19993) wifi:new:<10,0>, old:<10,0>, ap:<255,255>, sta:<10,0>, prof:1, snd_ch_cfg:0x0
I (20003) wifi:state: init -> auth (0xb0)
I (21013) wifi:state: auth -> init (0x200)
I (21023) wifi:new:<10,0>, old:<10,0>, ap:<255,255>, sta:<10,0>, prof:1, snd_ch_cfg:0x0
W (21023) greenhouse: Wi‑Fi disconnected, retrying...
W (22353) greenhouse: Wi‑Fi disconnected, retrying...
I (22453) wifi:new:<10,0>, old:<10,0>, ap:<255,255>, sta:<10,0>, prof:1, snd_ch_cfg:0x0
I (22453) wifi:state: init -> auth (0xb0)
I (23463) wifi:state: auth -> init (0x200)
I (23473) wifi:new:<10,0>, old:<10,0>, ap:<255,255>, sta:<10,0>, prof:1, snd_ch_cfg:0x0
W (23473) greenhouse: Wi‑Fi disconnected, retrying...
W (24803) greenhouse: Wi‑Fi disconnected, retrying...
I (24803) wifi:new:<10,0>, old:<10,0>, ap:<255,255>, sta:<10,0>, prof:1, snd_ch_cfg:0x0
I (24803) wifi:state: init -> auth (0xb0)
I (25823) wifi:state: auth -> init (0x200)
I (25833) wifi:new:<10,0>, old:<10,0>, ap:<255,255>, sta:<10,0>, prof:1, snd_ch_cfg:0x0
W (25833) greenhouse: Wi‑Fi disconnected, retrying...
W (27163) greenhouse: Wi‑Fi disconnected, retrying...
I (27163) wifi:new:<10,0>, old:<10,0>, ap:<255,255>, sta:<10,0>, prof:1, snd_ch_cfg:0x0
I (27173) wifi:state: init -> auth (0xb0)
I (28183) wifi:state: auth -> init (0x200)
I (28193) wifi:new:<10,0>, old:<10,0>, ap:<255,255>, sta:<10,0>, prof:1, snd_ch_cfg:0x0
W (28193) greenhouse: Wi‑Fi disconnected, retrying...
W (29523) greenhouse: Wi‑Fi disconnected, retrying...
I (29623) wifi:new:<10,0>, old:<10,0>, ap:<255,255>, sta:<10,0>, prof:1, snd_ch_cfg:0x0
I (29623) wifi:state: init -> auth (0xb0)
I (30633) wifi:state: auth -> init (0x200)
I (30643) wifi:new:<10,0>, old:<10,0>, ap:<255,255>, sta:<10,0>, prof:1, snd_ch_cfg:0x0
W (30643) greenhouse: Wi‑Fi disconnected, retrying...
W (31973) greenhouse: Wi‑Fi disconnected, retrying...
I (31983) wifi:new:<10,0>, old:<10,0>, ap:<255,255>, sta:<10,0>, prof:1, snd_ch_cfg:0x0
I (31983) wifi:state: init -> auth (0xb0)
I (32993) wifi:state: auth -> init (0x200)
I (33003) wifi:new:<10,0>, old:<10,0>, ap:<255,255>, sta:<10,0>, prof:1, snd_ch_cfg:0x0
W (33003) greenhouse: Wi‑Fi disconnected, retrying...
W (34333) greenhouse: Wi‑Fi disconnected, retrying...
I (34433) wifi:new:<10,0>, old:<10,0>, ap:<255,255>, sta:<10,0>, prof:1, snd_ch_cfg:0x0
I (34433) wifi:state: init -> auth (0xb0)

PS C:\Users\Carlo\workspace\dshot_esc> 
PS C:\Users\Carlo\workspace\dshot_esc> 
PS C:\Users\Carlo\workspace\dshot_esc> 
PS C:\Users\Carlo\workspace\dshot_esc> $env:IDF_PATH = 'C:\Users\Carlo\v5.5\esp-idf';
PS C:\Users\Carlo\workspace\dshot_esc>  & 'C:\Users\Carlo\.espressif\python_env\idf5.5_py3.11_env\Scripts\python.exe' 'C:\Users\Carlo\v5.5\esp-idf\tools\idf_monitor.py' -p COM6 -b 115200 --toolchain-prefix xtensa-esp32s3-elf- --make '''C:\Users\Carlo\.espressif\python_env\idf5.5_py3.11_env\Scripts\python.exe'' ''C:\Users\Carlo\v5.5\esp-idf\tools\idf.py''' --target esp32s3 'c:\Users\Carlo\workspace\dshot_esc\build\dshot_esc.elf'
--- Warning: GDB cannot open serial ports accessed as COMx
--- Using \\.\COM6 instead...
--- esp-idf-monitor 1.7.0 on \\.\COM6 115200
--- Quit: Ctrl+] | Menu: Ctrl+T | Help: Ctrl+T followed by Ctrl+H
ESP-ROM:esp32s3-20210327
Build:Mar 27 2021
rst:0x15 (USB_UART_CHIP_RESET),boot:0x28 (SPI_FAST_FLASH_BOOT)
Saved PC:0x4037acea
--- 0x4037acea: esp_cpu_wait_for_intr at C:/Users/Carlo/v5.5/esp-idf/components/esp_hw_support/cpu.c:64
SPIWP:0xee
mode:DIO, clock div:1
load:0x3fce2820,len:0x159c
load:0x403c8700,len:0xd24
load:0x403cb700,len:0x2f48
entry 0x403c8924
I (24) boot: ESP-IDF v5.5 2nd stage bootloader
I (24) boot: compile time Aug 20 2025 20:44:46
I (25) boot: Multicore bootloader
I (25) boot: chip revision: v0.2
I (27) boot: efuse block revision: v1.3
I (31) boot.esp32s3: Boot SPI Speed : 80MHz
I (35) boot.esp32s3: SPI Mode       : DIO
I (39) boot.esp32s3: SPI Flash Size : 2MB
I (42) boot: Enabling RNG early entropy source...
I (47) boot: Partition Table:
I (49) boot: ## Label            Usage          Type ST Offset   Length       
I (56) boot:  0 nvs              WiFi data        01 02 00009000 00006000     
I (62) boot:  1 phy_init         RF data          01 01 0000f000 00001000
I (69) boot:  2 factory          factory app      00 00 00010000 00100000     
I (75) boot: End of partition table
I (79) esp_image: segment 0: paddr=00010020 vaddr=3c0b0020 size=206c0h (132800) map
I (109) esp_image: segment 1: paddr=000306e8 vaddr=3fc9b700 size=04ce8h ( 19688) load
I (114) esp_image: segment 2: paddr=000353d8 vaddr=40374000 size=0ac40h ( 44096) load
I (124) esp_image: segment 3: paddr=00040020 vaddr=42000020 size=a31e8h (668136) map
I (243) esp_image: segment 4: paddr=000e3210 vaddr=4037ec40 size=0caa0h ( 51872) load
I (255) esp_image: segment 5: paddr=000efcb8 vaddr=600fe000 size=00020h (    32) load
I (265) boot: Loaded app from partition at offset 0x10000
I (265) boot: Disabling RNG early entropy source...
I (275) cpu_start: Multicore app
I (283) cpu_start: Pro cpu start user code
I (283) cpu_start: cpu freq: 160000000 Hz
I (284) app_init: Application information:
I (284) app_init: Project name:     dshot_esc
I (288) app_init: App version:      1
I (291) app_init: Compile time:     Aug 20 2025 20:44:37
I (296) app_init: ELF file SHA256:  bda5121e4...
I (300) app_init: ESP-IDF:          v5.5
I (304) efuse_init: Min chip rev:     v0.0
I (308) efuse_init: Max chip rev:     v0.99
I (312) efuse_init: Chip rev:         v0.2
I (316) heap_init: Initializing. RAM available for dynamic allocation:        
I (322) heap_init: At 3FCA48B8 len 00044E58 (275 KiB): RAM
I (327) heap_init: At 3FCE9710 len 00005724 (21 KiB): RAM
I (332) heap_init: At 3FCF0000 len 00008000 (32 KiB): DRAM
I (337) heap_init: At 600FE020 len 00001FC8 (7 KiB): RTCRAM
I (344) spi_flash: detected chip: generic
I (346) spi_flash: flash io: dio
W (349) spi_flash: Detected size(4096k) larger than the size in the binary image header(2048k). Using the size in the binary image header.
I (362) sleep_gpio: Configure to isolate all GPIO pins in sleep state
I (368) sleep_gpio: Enable automatic switching of GPIO sleep configuration    
I (375) main_task: Started on CPU0
I (385) main_task: Calling app_main()
I (385) greenhouse: Init NVS
I (465) greenhouse: Init Wi‑Fi
I (465) pp: pp rom version: e7ae62f
I (465) net80211: net80211 rom version: e7ae62f
I (475) wifi:wifi driver task: 3fced504, prio:23, stack:6656, core=0
I (485) wifi:wifi firmware version: f3dbad7
I (485) wifi:wifi certification version: v7.0
I (485) wifi:config NVS flash: enabled
I (485) wifi:config nano formatting: disabled
I (485) wifi:Init data frame dynamic rx buffer num: 32
I (495) wifi:Init static rx mgmt buffer num: 5
I (495) wifi:Init management short buffer num: 32
I (505) wifi:Init dynamic tx buffer num: 32
I (505) wifi:Init static tx FG buffer num: 2
I (505) wifi:Init static rx buffer size: 1600
I (515) wifi:Init static rx buffer num: 10
I (515) wifi:Init dynamic rx buffer num: 32
I (525) wifi_init: rx ba win: 6
I (525) wifi_init: accept mbox: 6
I (525) wifi_init: tcpip mbox: 32
I (525) wifi_init: udp mbox: 6
I (535) wifi_init: tcp mbox: 6
I (535) wifi_init: tcp tx win: 5760
I (535) wifi_init: tcp rx win: 5760
I (545) wifi_init: tcp mss: 1440
I (545) wifi_init: WiFi IRAM OP enabled
I (545) wifi_init: WiFi RX IRAM OP enabled
I (555) phy_init: phy_version 701,f4f1da3a,Mar  3 2025,15:50:10
I (595) wifi:mode : sta (f0:f5:bd:79:1b:a0)
I (595) wifi:enable tsf
I (595) wifi:Set ps type: 0, coexist: 0

I (595) greenhouse: Wi‑Fi start, connecting...

I (605) greenhouse: wifi_init_sta finished, waiting for IP...
I (635) wifi:new:<10,0>, old:<1,0>, ap:<255,255>, sta:<10,0>, prof:1, snd_ch_cfg:0x0
I (635) wifi:state: init -> auth (0xb0)
I (1645) wifi:state: auth -> init (0x200)
I (1655) wifi:new:<10,0>, old:<10,0>, ap:<255,255>, sta:<10,0>, prof:1, snd_ch_cfg:0x0
W (1655) greenhouse: Wi‑Fi disconnected, reason=2, retrying...
W (2985) greenhouse: Wi‑Fi disconnected, reason=205, retrying...
I (3095) wifi:new:<10,0>, old:<10,0>, ap:<255,255>, sta:<10,0>, prof:1, snd_ch_cfg:0x0
I (3095) wifi:state: init -> auth (0xb0)
I (4105) wifi:state: auth -> init (0x200)
I (4115) wifi:new:<10,0>, old:<10,0>, ap:<255,255>, sta:<10,0>, prof:1, snd_ch_cfg:0x0
W (4115) greenhouse: Wi‑Fi disconnected, reason=2, retrying...
W (5445) greenhouse: Wi‑Fi disconnected, reason=205, retrying...
I (5555) wifi:new:<10,0>, old:<10,0>, ap:<255,255>, sta:<10,0>, prof:1, snd_ch_cfg:0x0
I (5555) wifi:state: init -> auth (0xb0)
I (6565) wifi:state: auth -> init (0x200)
I (6575) wifi:new:<10,0>, old:<10,0>, ap:<255,255>, sta:<10,0>, prof:1, snd_ch_cfg:0x0
W (6575) greenhouse: Wi‑Fi disconnected, reason=2, retrying...
W (7905) greenhouse: Wi‑Fi disconnected, reason=205, retrying...
I (8005) wifi:new:<10,0>, old:<10,0>, ap:<255,255>, sta:<10,0>, prof:1, snd_ch_cfg:0x0
I (8005) wifi:state: init -> auth (0xb0)
I (9025) wifi:state: auth -> init (0x200)
I (9035) wifi:new:<10,0>, old:<10,0>, ap:<255,255>, sta:<10,0>, prof:1, snd_ch_cfg:0x0
W (9035) greenhouse: Wi‑Fi disconnected, reason=2, retrying...
W (10365) greenhouse: Wi‑Fi disconnected, reason=205, retrying...
I (10465) wifi:new:<10,0>, old:<10,0>, ap:<255,255>, sta:<10,0>, prof:1, snd_ch_cfg:0x0
I (10465) wifi:state: init -> auth (0xb0)
I (11475) wifi:state: auth -> init (0x200)
I (11485) wifi:new:<10,0>, old:<10,0>, ap:<255,255>, sta:<10,0>, prof:1, snd_ch_cfg:0x0
W (11485) greenhouse: Wi‑Fi disconnected, reason=2, retrying...
W (12815) greenhouse: Wi‑Fi disconnected, reason=205, retrying...
I (12825) wifi:new:<10,0>, old:<10,0>, ap:<255,255>, sta:<10,0>, prof:1, snd_ch_cfg:0x0
I (12825) wifi:state: init -> auth (0xb0)
I (13835) wifi:state: auth -> init (0x200)
I (13845) wifi:new:<10,0>, old:<10,0>, ap:<255,255>, sta:<10,0>, prof:1, snd_ch_cfg:0x0
W (13845) greenhouse: Wi‑Fi disconnected, reason=2, retrying...
W (15175) greenhouse: Wi‑Fi disconnected, reason=205, retrying...
I (15285) wifi:new:<10,0>, old:<10,0>, ap:<255,255>, sta:<10,0>, prof:1, snd_ch_cfg:0x0
I (15285) wifi:state: init -> auth (0xb0)
I (16295) wifi:state: auth -> init (0x200)
I (16305) wifi:new:<10,0>, old:<10,0>, ap:<255,255>, sta:<10,0>, prof:1, snd_ch_cfg:0x0
W (16305) greenhouse: Wi‑Fi disconnected, reason=2, retrying...
W (17635) greenhouse: Wi‑Fi disconnected, reason=205, retrying...
I (17735) wifi:new:<10,0>, old:<10,0>, ap:<255,255>, sta:<10,0>, prof:1, snd_ch_cfg:0x0
I (17735) wifi:state: init -> auth (0xb0)

PS C:\Users\Carlo\workspace\dshot_esc> 
PS C:\Users\Carlo\workspace\dshot_esc> 
PS C:\Users\Carlo\workspace\dshot_esc> 
PS C:\Users\Carlo\workspace\dshot_esc> 
PS C:\Users\Carlo\workspace\dshot_esc> $env:IDF_PATH = 'C:\Users\Carlo\v5.5\esp-idf';
PS C:\Users\Carlo\workspace\dshot_esc>  & 'C:\Users\Carlo\.espressif\python_env\idf5.5_py3.11_env\Scripts\python.exe' 'C:\Users\Carlo\v5.5\esp-idf\tools\idf_monitor.py' -p COM6 -b 115200 --toolchain-prefix xtensa-esp32s3-elf- --make '''C:\Users\Carlo\.espressif\python_env\idf5.5_py3.11_env\Scripts\python.exe'' ''C:\Users\Carlo\v5.5\esp-idf\tools\idf.py''' --target esp32s3 'c:\Users\Carlo\workspace\dshot_esc\build\dshot_esc.elf'
--- Warning: GDB cannot open serial ports accessed as COMx
--- Using \\.\COM6 instead...
--- esp-idf-monitor 1.7.0 on \\.\COM6 115200
--- Quit: Ctrl+] | Menu: Ctrl+T | Help: Ctrl+T followed by Ctrl+H
ESP-ROM:esp32s3-20210327
Build:Mar 27 2021
rst:0x15 (USB_UART_CHIP_RESET),boot:0x20 (DOWNLOAD(USB/UART0))
Saved PC:0x40041a79
--- 0x40041a79: ets_delay_us in ROM
waiting for download
--- Error: ClearCommError failed (PermissionError(13, 'The device does not recognize the command.', None, 22))
--- Waiting for the device to reconnect...................
I (1661) wifi:state: auth -> init (0x200)
I (1671) wifi:new:<10,0>, old:<10,0>, ap:<255,255>, sta:<10,0>, prof:1, snd_ch_cfg:0x0
W (1671) greenhouse: Wi‑Fi disconnected, reason=2 (AUTH_EXPIRE), retrying...  
W (3001) greenhouse: Wi‑Fi disconnected, reason=205 (CONNECTION_FAIL), retrying...
I (3111) wifi:new:<10,0>, old:<10,0>, ap:<255,255>, sta:<10,0>, prof:1, snd_ch_cfg:0x0
I (3111) wifi:state: init -> auth (0xb0)
I (3121) wifi:state: auth -> assoc (0x0)
I (3131) wifi:state: assoc -> run (0x10)
I (3171) wifi:connected with Tent, aid = 39, channel 10, BW20, bssid = 82:9c:25:89:44:45
I (3171) wifi:security: WPA2-PSK, phy: bgn, rssi: -51
I (3181) wifi:pm start, type: 0

I (3181) wifi:dp: 1, bi: 102400, li: 3, scale listen interval from 307200 us to 307200 us
I (3181) wifi:set rx beacon pti, rx_bcn_pti: 0, bcn_timeout: 25000, mt_pti: 0, mt_time: 10000
I (3191) greenhouse: Wi‑Fi connected to AP
I (3211) wifi:AP's beacon interval = 102400 us, DTIM period = 1
I (3221) wifi:<ba-add>idx:0 (ifx:0, 82:9c:25:89:44:45), tid:7, ssn:3, winSize:64
I (4221) esp_netif_handlers: sta ip: 192.168.53.147, mask: 255.255.255.0, gw: 192.168.53.1
I (4221) greenhouse: Got IP: 192.168.53.147, GW: 192.168.53.1, Netmask: 255.255.255.0
I (4221) greenhouse: Init modular config
I (4221) greenhouse: Init MQTT
I (4231) greenhouse: Init DShot motors (2)
I (5731) greenhouse: Setup complete
E (14241) esp-tls: [sock=54] select() timeout
E (14241) transport_base: Failed to open a new connection: 32774
E (14241) mqtt_client: Error transport connect
E (14241) greenhouse: MQTT error: link/down or auth failure
I (28141) wifi:state: run -> init (0x4c0)
I (28161) wifi:pm stop, total sleep time: 0 us / 24977528 us

I (28161) wifi:<ba-del>idx:0, tid:7
I (28161) wifi:new:<10,0>, old:<10,0>, ap:<255,255>, sta:<10,0>, prof:1, snd_ch_cfg:0x0
W (28161) greenhouse: Wi‑Fi disconnected, reason=4 (ASSOC_EXPIRE), retrying...
I (28201) wifi:new:<10,0>, old:<10,0>, ap:<255,255>, sta:<10,0>, prof:1, snd_ch_cfg:0x0
I (28201) wifi:state: init -> auth (0xb0)
I (29211) wifi:state: auth -> init (0x200)
I (29221) wifi:new:<10,0>, old:<10,0>, ap:<255,255>, sta:<10,0>, prof:1, snd_ch_cfg:0x0
W (29221) greenhouse: Wi‑Fi disconnected, reason=2 (AUTH_EXPIRE), retrying... 
E (29241) esp-tls: [sock=54] connect() error: Host is unreachable
E (29241) transport_base: Failed to open a new connection: 32772
E (29241) mqtt_client: Error transport connect
E (29241) greenhouse: MQTT error: link/down or auth failure
W (30551) greenhouse: Wi‑Fi disconnected, reason=205 (CONNECTION_FAIL), retrying...
I (30661) wifi:new:<10,0>, old:<10,0>, ap:<255,255>, sta:<10,0>, prof:1, snd_ch_cfg:0x0
I (30661) wifi:state: init -> auth (0xb0)
I (31671) wifi:state: auth -> init (0x200)
I (31681) wifi:new:<10,0>, old:<10,0>, ap:<255,255>, sta:<10,0>, prof:1, snd_ch_cfg:0x0
W (31681) greenhouse: Wi‑Fi disconnected, reason=2 (AUTH_EXPIRE), retrying...
W (33011) greenhouse: Wi‑Fi disconnected, reason=205 (CONNECTION_FAIL), retrying...
I (33131) wifi:new:<10,0>, old:<10,0>, ap:<255,255>, sta:<10,0>, prof:1, snd_ch_cfg:0x0
I (33131) wifi:state: init -> auth (0xb0)
I (34141) wifi:state: auth -> init (0x200)
I (34151) wifi:new:<10,0>, old:<10,0>, ap:<255,255>, sta:<10,0>, prof:1, snd_ch_cfg:0x0
W (34151) greenhouse: Wi‑Fi disconnected, reason=2 (AUTH_EXPIRE), retrying...
W (35481) greenhouse: Wi‑Fi disconnected, reason=205 (CONNECTION_FAIL), retrying...
I (35581) wifi:new:<10,0>, old:<10,0>, ap:<255,255>, sta:<10,0>, prof:1, snd_ch_cfg:0x0
I (35581) wifi:state: init -> auth (0xb0)
I (36591) wifi:state: auth -> init (0x200)
I (36601) wifi:new:<10,0>, old:<10,0>, ap:<255,255>, sta:<10,0>, prof:1, snd_ch_cfg:0x0
W (36601) greenhouse: Wi‑Fi disconnected, reason=2 (AUTH_EXPIRE), retrying... 
W (37931) greenhouse: Wi‑Fi disconnected, reason=205 (CONNECTION_FAIL), retrying...
I (38031) wifi:new:<10,0>, old:<10,0>, ap:<255,255>, sta:<10,0>, prof:1, snd_ch_cfg:0x0
I (38031) wifi:state: init -> auth (0xb0)
I (39041) wifi:state: auth -> init (0x200)
I (39091) wifi:new:<10,0>, old:<10,0>, ap:<255,255>, sta:<10,0>, prof:1, snd_ch_cfg:0x0
W (39091) greenhouse: Wi‑Fi disconnected, reason=2 (AUTH_EXPIRE), retrying... 
W (40421) greenhouse: Wi‑Fi disconnected, reason=205 (CONNECTION_FAIL), retrying...
I (40491) wifi:new:<10,0>, old:<10,0>, ap:<255,255>, sta:<10,0>, prof:1, snd_ch_cfg:0x0
I (40491) wifi:state: init -> auth (0xb0)
I (41501) wifi:state: auth -> init (0x200)
I (41511) wifi:new:<10,0>, old:<10,0>, ap:<255,255>, sta:<10,0>, prof:1, snd_ch_cfg:0x0
W (41511) greenhouse: Wi‑Fi disconnected, reason=2 (AUTH_EXPIRE), retrying...
W (42841) greenhouse: Wi‑Fi disconnected, reason=205 (CONNECTION_FAIL), retrying...
I (42951) wifi:new:<10,0>, old:<10,0>, ap:<255,255>, sta:<10,0>, prof:1, snd_ch_cfg:0x0
I (42951) wifi:state: init -> auth (0xb0)
I (43961) wifi:state: auth -> init (0x200)
I (43971) wifi:new:<10,0>, old:<10,0>, ap:<255,255>, sta:<10,0>, prof:1, snd_ch_cfg:0x0
W (43971) greenhouse: Wi‑Fi disconnected, reason=2 (AUTH_EXPIRE), retrying... 
E (44251) esp-tls: [sock=54] connect() error: Host is unreachable
E (44251) transport_base: Failed to open a new connection: 32772
E (44251) mqtt_client: Error transport connect
E (44251) greenhouse: MQTT error: link/down or auth failure
W (45301) greenhouse: Wi‑Fi disconnected, reason=205 (CONNECTION_FAIL), retrying...
I (45401) wifi:new:<10,0>, old:<10,0>, ap:<255,255>, sta:<10,0>, prof:1, snd_ch_cfg:0x0
I (45401) wifi:state: init -> auth (0xb0)
