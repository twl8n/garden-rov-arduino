```bash
arduino-cli core -h
```

```bash
arduino-cli core --additional-urls https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
```

```bash
arduino-cli core search esp32
```


```console
ID            Version Name
arduino:esp32 2.0.13  Arduino ESP32 Boards
esp32:esp32   3.0.4   esp32
```

```bash
arduino-cli core install esp32:esp32
```


```bash
arduino-cli core list
```


```console
ID          Installed Latest Name
arduino:avr 1.8.6     1.8.6  Arduino AVR Boards
esp32:esp32 3.0.4     3.0.4  esp32
```

```bash
arduino-cli board search esp32
```


```bash
arduino-cli board search esp32 | less
```


```console
Board Name                                         FQBN                                               Platform ID
4D Systems gen4-ESP32 16MB Modules (ESP32-S3R8n16) esp32:esp32:gen4-ESP32-S3R8n16                     esp32:esp32
AI Thinker ESP32-CAM                               esp32:esp32:esp32cam                               esp32:esp32
ALKS ESP32                                         esp32:esp32:alksesp32                              esp32:esp32
...
```

```bash
arduino-cli board list
```


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

```bash
arduino-cli monitor -p /dev/cu.usbserial-210 --config 115200 -b esp32:esp32:esp32cam
```

#### Device images

The ESP32-CAM MCU/camera board separated from the ESP32-CAM-MB programmer/USB:

<img src="images/IMG_4222_2.JPG" alt="ESP32-CAM-MB" width="640"/>

The ESP32-CAM camera ribbon cable latch pivots, unlike Raspberry PI and camera boards where the latch slides.

Flip the latch up to open, and insert the camera's ribbon cable.

Flip down to latch and secure.

<img src="images/DSC_6992_2.JPG" alt="ribbon cable latch open" width="640">

<img src="images/DSC_6996_2.JPG" alt="ribbon cable latch closed" width="640">


