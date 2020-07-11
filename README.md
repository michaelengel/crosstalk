Smalltalk-80 for Raspberry Pi version 0.2
This is a bare metal Smalltalk-80 port to the Raspberry Pi.

See docs/Changelog.txt for updates.

Based on the Smalltalk-80 C++ implementation by Dan Banay (https://github.com/dbanay/Smalltalk)
and the circle bare metal library by Rene Stange (https://github.com/rsta2/circle)

Tested hardware (known working):
- Raspberry Pi 1B and Zero W
- Raspberry Pi 2B V 1.1 (with BCM2836)
- Raspberry Pi 3B V 1.2
- Raspberry Pi 4B (4 GB version)

Tested hardware (NOT working):
- Raspberry Pi 4 (8 GB version)

Required additional hardware:
- HDMI screen with 1920x1080 resolution
- USB mouse/keyboard (USB hub via OTG on Raspberry Pi Zero works)

Building is tested on MacOS X 10.14 with arm-none-eabi toolchain installed via homebrew

To build, simply execute ./build.sh in the smalltalk-raspi-circle directory.
GNU make 4.0 or newer is required to build circle (install via homebrew).

This builds kernels for 
- Raspberry Pi 1B/Zero (kernel.img)
- Raspberry Pi 2B/3B   (kernel7.img)
- Raspberry Pi 4B      (kernel7l.img)

To use, copy the contents of the "sdboot" directory to the boot FAT partition of an SD card.

Enjoy!
-- Michael

