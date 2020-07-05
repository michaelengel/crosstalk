#!/bin/sh

function build {
  pushd . > /dev/null 2>&1

  cd circle
  rm ./Config.mk
  echo "RASPPI = "$1 > Config.mk
  echo "PREFIX = arm-none-eabi-" >> Config.mk

  echo "Building circle library"
  ./makeall clean > build_$1.log 2>&1
  ./makeall --nosample >> build_$1.log 2>&1

  popd > /dev/null 2>&1
  pushd . > /dev/null 2>&1

  cd circle/addon/SDCard/
  echo "Building circle addon SDcard"
  make clean >> build_$1.log 2>&1
  make >> build_$1.log 2>&1

  popd > /dev/null 2>&1
  pushd . > /dev/null 2>&1

  cd circle/addon/fatfs/
  echo "Building circle addon fatfs"
  make clean >> build_$1.log 2>&1
  make >> build_$1.log 2>&1

  popd  > /dev/null 2>&1
  pushd . > /dev/null 2>&1

  cd smalltalk/raspberry/
  make clean >> build_$1.log 2>&1
  make >> build_$1.log 2>&1
 
  popd > /dev/null 2>&1
}

### building starts here

# Raspberry Pi 1/Zero
echo "Building Smalltalk-80 bare metal for Raspberry Pi 1/Zero"
build 1 
cp smalltalk/raspberry/kernel.img sdboot/
echo

# Raspberry Pi 2
echo "Building Smalltalk-80 bare metal for Raspberry Pi 2"
build 2 
cp smalltalk/raspberry/kernel7.img sdboot/
echo

# Raspberry Pi 3
echo "Building Smalltalk-80 bare metal for Raspberry Pi 3"
build 3 
cp smalltalk/raspberry/kernel8-32.img sdboot/
echo

# Raspberry Pi 4 not yet supported, sorry

echo "Finished"

