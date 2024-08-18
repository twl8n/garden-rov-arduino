# garden-rov-arduino
UGV for garden cultivation, remote control via WiFi, featuring Arduino and Pi Zero W with streaming camera.

# Install arduino-cli on Raspberry Pi Zero W

# arduino cli on RPi zero, apparently no apt package?
curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh

wget https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh

twl@raspberrypi:~ $ wget https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh
--2024-08-16 03:04:39--  https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh
Resolving raw.githubusercontent.com (raw.githubusercontent.com)... 2606:50c0:8003::154, 2606:50c0:8002::154, 2606:50c0:8001::154, ...
Connecting to raw.githubusercontent.com (raw.githubusercontent.com)|2606:50c0:8003::154|:443... connected.
HTTP request sent, awaiting response... 200 OK
Length: 8357 (8.2K) [text/plain]
Saving to: ‘install.sh’

install.sh                            100%[=======================================================================>]   8.16K  --.-KB/s    in 0.005s

2024-08-16 03:04:40 (1.51 MB/s) - ‘install.sh’ saved [8357/8357]

twl@raspberrypi:~ $ id
uid=1000(twl) gid=1000(twl) groups=1000(twl),4(adm),20(dialout),24(cdrom),27(sudo),29(audio),44(video),46(plugdev),60(games),100(users),102(input),105(render),106(netdev),993(gpio),994(i2c),995(spi)
twl@raspberrypi:~ $ source ./install.sh
Installing in /home/twl/bin
ARCH=ARMv6
OS=Linux
Using curl as download tool
Downloading https://downloads.arduino.cc/arduino-cli/arduino-cli_1.0.4_Linux_ARMv6.tar.gz
install.sh: arduino-cli not found. You might want to add "/home/twl/bin" to your $PATH
arduino-cli  Version: 1.0.4 Commit: a0d912da Date: 2024-08-12T13:42:40Z installed successfully in /home/twl/bin
twl@raspberrypi:~ $
...
Failed to install arduino-cli
-bash: pop_var_context: head of shell_variables not a function context
Connection to raspberrypi.local closed.
...
twl@raspberrypi:~ $ arduino-cli config init
Config file written to: /home/twl/.arduino15/arduino-cli.yaml
twl@raspberrypi:~ $ arduino-cli core update-index
Downloading index: library_index.tar.bz2 downloaded
Downloading index: package_index.tar.bz2 downloaded
Downloading missing tool builtin:ctags@5.8-arduino11...
builtin:ctags@5.8-arduino11 downloaded
Installing builtin:ctags@5.8-arduino11...
Skipping tool configuration....
builtin:ctags@5.8-arduino11 installed
Downloading missing tool builtin:dfu-discovery@0.1.2...
builtin:dfu-discovery@0.1.2 downloaded
Installing builtin:dfu-discovery@0.1.2...
Skipping tool configuration....
builtin:dfu-discovery@0.1.2 installed
Downloading missing tool builtin:mdns-discovery@1.0.9...
builtin:mdns-discovery@1.0.9 downloaded
Installing builtin:mdns-discovery@1.0.9...
Skipping tool configuration....
builtin:mdns-discovery@1.0.9 installed
Downloading missing tool builtin:serial-discovery@1.4.1...
builtin:serial-discovery@1.4.1 downloaded
Installing builtin:serial-discovery@1.4.1...
Skipping tool configuration....
builtin:serial-discovery@1.4.1 installed
Downloading missing tool builtin:serial-monitor@0.14.1...
builtin:serial-monitor@0.14.1 downloaded
Installing builtin:serial-monitor@0.14.1...
Skipping tool configuration....
builtin:serial-monitor@0.14.1 installed
Downloading index: package_index.tar.bz2 downloaded
twl@raspberrypi:~ $ arduino-cli board list
Port         Protocol Type              Board Name  FQBN            Core
/dev/ttyACM0 serial   Serial Port (USB) Arduino Uno arduino:avr:uno arduino:avr
twl@raspberrypi:~ $ arduino-cli core list
No platforms installed.
twl@raspberrypi:~ $ arduino-cli core install arduino:avr
Downloading packages...
arduino:arduinoOTA@1.3.0 downloaded
arduino:avr-gcc@7.3.0-atmel3.6.1-arduino7 downloaded
arduino:avrdude@6.3.0-arduino17 downloaded
arduino:avr@1.8.6 downloaded
Installing arduino:arduinoOTA@1.3.0...
Configuring tool....
arduino:arduinoOTA@1.3.0 installed
Installing arduino:avr-gcc@7.3.0-atmel3.6.1-arduino7...
Configuring tool....
arduino:avr-gcc@7.3.0-atmel3.6.1-arduino7 installed
Installing arduino:avrdude@6.3.0-arduino17...
Configuring tool....
arduino:avrdude@6.3.0-arduino17 installed
Installing platform arduino:avr@1.8.6...
Configuring platform....
Platform arduino:avr@1.8.6 installed
twl@raspberrypi:~ $ arduino-cli core list
ID          Installed Latest Name
arduino:avr 1.8.6     1.8.6  Arduino AVR Boards

arduino-cli monitor -p /dev/ttyACM0 -b arduino:avr:uno

arduino-cli lib install "Cytron Motor Drivers Library"
Downloading Cytron Motor Drivers Library@1.0.1...
Cytron Motor Drivers Library@1.0.1 downloaded
Installing Cytron Motor Drivers Library@1.0.1...
Installed Cytron Motor Drivers Library@1.0.1

arduino-cli compile --fqbn arduino:avr:uno buggy
twl@raspberrypi:~ $ arduino-cli compile --fqbn arduino:avr:uno buggy
Sketch uses 1644 bytes (5%) of program storage space. Maximum is 32256 bytes.
Global variables use 17 bytes (0%) of dynamic memory, leaving 2031 bytes for local variables. Maximum is 2048 bytes.

Used library                 Version Path
Cytron Motor Drivers Library 1.0.1   /home/twl/Arduino/libraries/Cytron_Motor_Drivers_Library

Used platform Version Path
arduino:avr   1.8.6   /home/twl/.arduino15/packages/arduino/hardware/avr/1.8.6

# The final arg is path relative to cwd, *not* object name or project name.
# arduino-cli upload -p /dev/ttyACM0 --fqbn arduino:avr:uno buggy

arduino-cli compile -e --fqbn arduino:avr:uno .
# now a build dir exists in cwd

twl@raspberrypi:~ $ arduino-cli upload -p /dev/ttyACM0 --fqbn arduino:avr:uno buggy
New upload port: /dev/ttyACM0 (serial)

twl@raspberrypi:~ $ arduino-cli upload -v -p /dev/ttyACM0 --fqbn arduino:avr:uno buggy

avrdude: Version 6.3-20190619
         Copyright (c) 2000-2005 Brian Dean, http://www.bdmicro.com/
         Copyright (c) 2007-2014 Joerg Wunsch

         System wide configuration file is "/home/twl/.arduino15/packages/arduino/tools/avrdude/6.3.0-arduino17/etc/avrdude.conf"
"/home/twl/.arduino15/packages/arduino/tools/avrdude/6.3.0-arduino17/bin/avrdude" "-C/home/twl/.arduino15/packages/arduino/tools/avrdude/6.3.0-arduino17/etc/avrdude.conf" -v -V -patmega328p -carduino "-P/dev/ttyACM0" -b115200 -D "-Uflash:w:/tmp/arduino/sketches/6C361DC52CF963E58E6EF93D4417CC4E/buggy.ino.hex:i"
         User configuration file is "/home/twl/.avrduderc"
         User configuration file does not exist or is not a regular file, skipping

         Using Port                    : /dev/ttyACM0
         Using Programmer              : arduino
         Overriding Baud Rate          : 115200
         AVR Part                      : ATmega328P
         Chip Erase delay              : 9000 us
         PAGEL                         : PD7
         BS2                           : PC2
         RESET disposition             : dedicated
         RETRY pulse                   : SCK
         serial program mode           : yes
         parallel program mode         : yes
         Timeout                       : 200
         StabDelay                     : 100
         CmdexeDelay                   : 25
         SyncLoops                     : 32
         ByteDelay                     : 0
         PollIndex                     : 3
         PollValue                     : 0x53
         Memory Detail                 :

                                  Block Poll               Page                       Polled
           Memory Type Mode Delay Size  Indx Paged  Size   Size #Pages MinW  MaxW   ReadBack
           ----------- ---- ----- ----- ---- ------ ------ ---- ------ ----- ----- ---------
           eeprom        65    20     4    0 no       1024    4      0  3600  3600 0xff 0xff
           flash         65     6   128    0 yes     32768  128    256  4500  4500 0xff 0xff
           lfuse          0     0     0    0 no          1    0      0  4500  4500 0x00 0x00
           hfuse          0     0     0    0 no          1    0      0  4500  4500 0x00 0x00
           efuse          0     0     0    0 no          1    0      0  4500  4500 0x00 0x00
           lock           0     0     0    0 no          1    0      0  4500  4500 0x00 0x00
           calibration    0     0     0    0 no          1    0      0     0     0 0x00 0x00
           signature      0     0     0    0 no          3    0      0     0     0 0x00 0x00

         Programmer Type : Arduino
         Description     : Arduino
         Hardware Version: 3
         Firmware Version: 4.4
         Vtarget         : 0.3 V
         Varef           : 0.3 V
         Oscillator      : 28.800 kHz
         SCK period      : 3.3 us

avrdude: AVR device initialized and ready to accept instructions

Reading | ################################################## | 100% 0.00s

avrdude: Device signature = 0x1e950f (probably m328p)
avrdude: reading input file "/tmp/arduino/sketches/6C361DC52CF963E58E6EF93D4417CC4E/buggy.ino.hex"
avrdude: writing flash (1644 bytes):

Writing | ################################################## | 100% 0.31s

avrdude: 1644 bytes of flash written

avrdude done.  Thank you.

New upload port: /dev/ttyACM0 (serial)

# Install arduino-cli on MacOS

# install, choose board
> arduino-cli config init
Config file written to: /Users/twl/Library/Arduino15/arduino-cli.yamlt
> arduino-cli core update-index
Downloading index: package_index.tar.bz2 downloaded
> arduino-cli board list
Port                            Protocol Type              Board Name  FQBN            Core
/dev/cu.Bluetooth-Incoming-Port serial   Serial Port       Unknown
/dev/cu.usbmodem2101            serial   Serial Port (USB) Arduino Uno arduino:avr:uno arduino:avr
/dev/cu.wlan-debug              serial   Serial Port       Unknown
> arduino-cli core install arduino:avr
Platform arduino:avr@1.8.6 already installed
> arduino-cli core list
ID          Installed Latest Name
arduino:avr 1.8.6     1.8.6  Arduino AVR Boards

--
https://arduino.github.io/arduino-cli/1.0/getting-started/
Fully Qualified Board Name
brew update
brew upgrade
brew install arduino-cli
arduino-cli config init
Config file written to: /Users/twl/Library/Arduino15/arduino-cli.yamlt
# a fresh install is to update the local cache of available platforms and libraries
arduino-cli core update-index
arduino-cli board list

> arduino-cli board list
Port                            Protocol Type              Board Name  FQBN            Core
/dev/cu.Bluetooth-Incoming-Port serial   Serial Port       Unknown
/dev/cu.usbmodem2101            serial   Serial Port (USB) Arduino Uno arduino:avr:uno arduino:avr
/dev/cu.wlan-debug              serial   Serial Port       Unknown

# assuming the FQBN is arduino:samd
arduino-cli core install arduino:samd
arduino-cli core install arduino:avr:uno # didn't work, need "core" not "fqbn"
arduino-cli core install arduino:avr

> arduino-cli core install arduino:avr
Platform arduino:avr@1.8.6 already installed

arduino-cli core list

> arduino-cli core list
ID          Installed Latest Name
arduino:avr 1.8.6     1.8.6  Arduino AVR Boards

arduino-cli compile --fqbn arduino:samd:mkr1000 MyFirstSketch

# need the fqbn for compile:
arduino-cli compile --fqbn arduino:avr:uno dual_servo.ino

> arduino-cli compile --fqbn arduino:avr:uno dual_servo.ino
/Users/twl/src/Arduino/dual_servo/dual_servo.ino:1:10: fatal error: Servo.h: No such file or directory
 #include <Servo.h>
          ^~~~~~~~~
compilation terminated.

Used platform Version Path
arduino:avr   1.8.6   /Users/twl/Library/Arduino15/packages/arduino/hardware/avr/1.8.6
Error during build: exit status 1

> arduino-cli lib update-index
Downloading index: library_index.tar.bz2 downloaded
# ok, downloaded, but was the library installed?? Apparently not.

> arduino-cli lib install Servo
Downloading Servo@1.2.2...
Servo@1.2.2 Servo@1.2.2 already downloaded
Installing Servo@1.2.2...
Installed Servo@1.2.2
# why "already downloaded"?

> arduino-cli compile --fqbn arduino:avr:uno dual_servo.ino
Sketch uses 5692 bytes (17%) of program storage space. Maximum is 32256 bytes.
Global variables use 720 bytes (35%) of dynamic memory, leaving 1328 bytes for local variables. Maximum is 2048 bytes.

Used library Version Path
Servo        1.2.2   /Users/twl/Documents/Arduino/libraries/Servo

Used platform Version Path
arduino:avr   1.8.6   /Users/twl/Library/Arduino15/packages/arduino/hardware/avr/1.8.6

# compile and upload
arduino-cli compile -u -p /dev/cu.usbmodem2101 --fqbn arduino:avr:uno .


# upload example
arduino-cli upload -p /dev/ttyACM0 --fqbn arduino:samd:mkr1000 MyFirstSketch

arduino-cli upload -p /dev/cu.usbmodem2101 --fqbn arduino:avr:uno dual_servo

> arduino-cli upload -p /dev/cu.usbmodem2101 --fqbn arduino:avr:uno dual_servo
Error during Upload: Can't open sketch: no such file or directory: /Users/twl/src/Arduino/dual_servo/dual_servo

# OMG, the final arg is path relative to cwd, *not* object name or project name.
> arduino-cli upload -p /dev/cu.usbmodem2101 --fqbn arduino:avr:uno .
New upload port: /dev/cu.usbmodem2101 (serial)

# terminal monitor
arduino-cli monitor -p /dev/cu.usbmodem2101 -b arduino:avr:uno
# probably need --raw to read without cr/lf buffering
arduino-cli monitor --raw -p /dev/cu.usbmodem2101 -b arduino:avr:uno

`stty sane^J # take the terminal out of raw mode.`

Compile on Mac, scp buggy.ino.hex to rpi, upload from rpi to uno

`
cd src/Arduino/buggy
arduino-cli compile -e --fqbn arduino:avr:uno .
scp build/arduino.avr.uno/buggy.ino.hex raspberrypi.local:
arduino-cli upload -p /dev/ttyACM0 --fqbn arduino:avr:uno -i buggy.ino.hex
`

twl@raspberrypi:~ $ arduino-cli upload -p /dev/ttyACM0 --fqbn arduino:avr:uno -i buggy.ino.hex
New upload port: /dev/ttyACM0 (serial)

# Library install and cli problem solving

> arduino-cli version
arduino-cli  Version: 1.0.3 Commit: Homebrew Date: 2024-07-23T08:43:59Z

library, compile, upload
> cd src/Arduino/dual_servo
> arduino-cli compile --fqbn arduino:avr:uno dual_servo.ino
/Users/twl/src/Arduino/dual_servo/dual_servo.ino:1:10: fatal error: Servo.h: No such file or directory
 #include <Servo.h>
          ^~~~~~~~~
compilation terminated.

Used platform Version Path
arduino:avr   1.8.6   /Users/twl/Library/Arduino15/packages/arduino/hardware/avr/1.8.6
Error during build: exit status 1

> arduino-cli lib search servo | grep Name
# ... long list of stuff, including Servo
# arduino-cli lib search servo | less
# ... long blather of stuff, but Servo happens to be the first entry.

> arduino-cli lib install Servo
Downloading Servo@1.2.2...
Servo@1.2.2 Servo@1.2.2 already downloaded
Installing Servo@1.2.2...
Installed Servo@1.2.2

> arduino-cli compile --fqbn arduino:avr:uno dual_servo.ino
Sketch uses 5692 bytes (17%) of program storage space. Maximum is 32256 bytes.
Global variables use 720 bytes (35%) of dynamic memory, leaving 1328 bytes for local variables. Maximum is 2048 bytes.

Used library Version Path
Servo        1.2.2   /Users/twl/Documents/Arduino/libraries/Servo

Used platform Version Path
arduino:avr   1.8.6   /Users/twl/Library/Arduino15/packages/arduino/hardware/avr/1.8.6

> pwd
/Users/twl/src/Arduino/dual_servo

# The final arg is path relative to cwd, *not* object name or project name.
> arduino-cli upload -p /dev/cu.usbmodem2101 --fqbn arduino:avr:uno .
New upload port: /dev/cu.usbmodem2101 (serial)

arduino-cli compile -u -p /dev/cu.usbmodem2101 --fqbn arduino:avr:uno dual_servo.ino
# 
arduino-cli compile -u -p /dev/cu.usbmodem2101 --fqbn arduino:avr:uno .

