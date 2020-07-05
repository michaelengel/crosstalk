crosstalk: Smalltalk-80 for Raspberry Pi ("Smalltalk - The Next Generation")

This is version 0.1 (code name "Scotty" - everything works, but it's hacked together),
the first public version of my bare metal Smalltalk-80 port to the Raspberry Pi.

Based on the Smalltalk-80 C++ implementation by Dan Banay (https://github.com/dbanay/Smalltalk)
and the circle bare metal library by Rene Stange (https://github.com/rsta2/circle).

Tested systems (known working):
- Raspberry Pi 1B V 1.2 (512 MB RAM) and Zero W
- Raspberry Pi 2B V 1.1 (with BCM2836 SoC)
- Raspberry Pi 3B V 1.2

Tested systems (NOT working):
- Raspberry Pi 4 (4 and 8 GB versions)

Required additional hardware:
- HDMI screen with 1920x1080 resolution
- USB mouse/keyboard (USB hub via OTG on Raspberry Pi Zero works)

Building is tested on MacOS X 10.14 with arm-none-eabi toolchain installed via homebrew

To build, simply execute ./build.sh in the smalltalk-raspi-circle directory.

This builds kernels for 
- Raspberry Pi 1B/Zero (kernel.img)
- Raspberry Pi 2B      (kernel7.img)
- Raspberry Pi 3B      (kernel8-32.img)

To use, copy the contents of the "sdboot" directory to the boot FAT partition of an SD card.

Enjoy!
-- Michael

