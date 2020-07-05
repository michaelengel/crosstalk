#
# Makefile
#

CIRCLEHOME = ../../circle

CFLAGS  += -DSOFTWARE_MOUSE_CURSOR -Wno-strict-aliasing -mfloat-abi=softfp # -finstrument-functions
# CFLAGS  += -Wno-strict-aliasing -mfloat-abi=softfp # -finstrument-functions

INCLUDE += -I$(CIRCLEHOME)/lib/fs/fat -I../src -I.

OBJS	= main.o kernel.o ../src/interpreter.o ../src/objmemory.o ../src/bitblt.o ../src/smalltalk.o syscalls.o

LIBS	= $(CIRCLEHOME)/lib/usb/libusb.a \
	  $(CIRCLEHOME)/lib/input/libinput.a \
	  $(CIRCLEHOME)/addon/SDCard/libsdcard.a \
	  $(CIRCLEHOME)/addon/fatfs/libfatfs.a \
	  $(CIRCLEHOME)/lib/fs/libfs.a \
	  $(CIRCLEHOME)/lib/libcircle.a \
	  /usr/local/Cellar/arm-gcc-bin/9-2019-q4-major/arm-none-eabi/lib/libc.a \
	  /usr/local/Cellar/arm-gcc-bin/9-2019-q4-major/arm-none-eabi/lib/libstdc++.a

include ../Rules.mk

-include $(DEPS)
