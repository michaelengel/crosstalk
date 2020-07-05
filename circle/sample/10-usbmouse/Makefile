#
# Makefile
#

CIRCLEHOME = ../..

OBJS	= main.o kernel.o

LIBS	= $(CIRCLEHOME)/lib/usb/libusb.a \
	  $(CIRCLEHOME)/lib/input/libinput.a \
	  $(CIRCLEHOME)/lib/fs/libfs.a \
	  $(CIRCLEHOME)/lib/libcircle.a \
          /usr/local/Cellar/arm-gcc-bin/9-2019-q4-major/arm-none-eabi/lib/libc.a \
          /usr/local/Cellar/arm-gcc-bin/9-2019-q4-major/arm-none-eabi/lib/libstdc++.a


include ../Rules.mk

-include $(DEPS)
