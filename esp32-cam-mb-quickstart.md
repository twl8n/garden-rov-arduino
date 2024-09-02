```arduino-cli core -h```

```arduino-cli core --additional-urls https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json```

```arduino-cli core search esp32```

```console
ID            Version Name
arduino:esp32 2.0.13  Arduino ESP32 Boards
esp32:esp32   3.0.4   esp32
```

`arduino-cli core install esp32:esp32`

`arduino-cli core list`

```console
ID          Installed Latest Name
arduino:avr 1.8.6     1.8.6  Arduino AVR Boards
esp32:esp32 3.0.4     3.0.4  esp32
```

`arduino-cli board search esp32`

`arduino-cli board search esp32 | less`

```console
Board Name                                         FQBN                                               Platform ID
4D Systems gen4-ESP32 16MB Modules (ESP32-S3R8n16) esp32:esp32:gen4-ESP32-S3R8n16                     esp32:esp32
AI Thinker ESP32-CAM                               esp32:esp32:esp32cam                               esp32:esp32
ALKS ESP32                                         esp32:esp32:alksesp32                              esp32:esp32
...
```

`arduino-cli board list`

When the esp32 is not plugged into USB:

```console
Port                            Protocol Type        Board Name FQBN Core
/dev/cu.Bluetooth-Incoming-Port serial   Serial Port Unknown
/dev/cu.wlan-debug              serial   Serial Port Unknown
```
When the esp32 **is** plugged into USB:

```console
Port                            Protocol Type              Board Name FQBN Core
/dev/cu.Bluetooth-Incoming-Port serial   Serial Port       Unknown
/dev/cu.usbserial-210           serial   Serial Port (USB) Unknown
/dev/cu.wlan-debug              serial   Serial Port       Unknown
```

`arduino-cli monitor -p /dev/cu.usbserial-210 --config 115200 -b esp32:esp32:esp32cam`
