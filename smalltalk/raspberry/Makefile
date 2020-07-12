#
# Makefile
#

CIRCLEHOME = ../../circle

CFLAGS  += -Wno-strict-aliasing -mfloat-abi=softfp

INCLUDE += -I$(CIRCLEHOME)/lib/fs/fat -I../src -I.

OBJS	= main.o kernel.o ../src/interpreter.o ../src/objmemory.o ../src/bitblt.o ../src/smalltalk.o syscalls.o

LIBS	= $(CIRCLEHOME)/lib/usb/libusb.a \
	  $(CIRCLEHOME)/lib/input/libinput.a \
	  $(CIRCLEHOME)/addon/SDCard/libsdcard.a \
	  $(CIRCLEHOME)/addon/fatfs/libfatfs.a \
	  $(CIRCLEHOME)/lib/fs/libfs.a \
	  $(CIRCLEHOME)/lib/libcircle.a 

include ../Rules.mk

-include $(DEPS)
