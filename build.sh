#!/bin/bash

function clean {
  pushd . > /dev/null 2>&1

  cd circle
  rm ./Config.mk
  echo "RASPPI = "$1 > Config.mk
  echo "PREFIX = arm-none-eabi-" >> Config.mk

  echo "Cleaning circle library"
  ./makeall clean
  rm build*.log

  popd > /dev/null 2>&1
  pushd . > /dev/null 2>&1

  cd circle/addon/SDCard/
  echo "Cleaning circle addon SDcard"
  make clean
  rm build*.log

  popd > /dev/null 2>&1
  pushd . > /dev/null 2>&1

  cd circle/addon/fatfs/
  echo "Cleaning circle addon fatfs"
  make clean
  rm build*.log

  popd  > /dev/null 2>&1
  pushd . > /dev/null 2>&1

  cd smalltalk/raspberry/
  make clean
  rm build*.log
  rm kernel*.img
  rm kernel*.map
 
  popd > /dev/null 2>&1
}

function build {
  pushd . > /dev/null 2>&1

  cd circle
  rm ./Config.mk
  echo "RASPPI = "$1 > Config.mk
  echo "PREFIX = arm-none-eabi-" >> Config.mk

  echo "Building circle library"
  ./makeall clean > build_$1.log 2>&1
  ./makeall --nosample > build_$1.log 2>&1

  popd > /dev/null 2>&1
  pushd . > /dev/null 2>&1

  cd circle/addon/SDCard/
  echo "Building circle addon SDcard"
  make clean >> build_$1.log 2>&1
  make > build_$1.log 2>&1

  popd > /dev/null 2>&1
  pushd . > /dev/null 2>&1

  cd circle/addon/fatfs/
  echo "Building circle addon fatfs"
  make clean >> build_$1.log 2>&1
  make > build_$1.log 2>&1

  popd  > /dev/null 2>&1
  pushd . > /dev/null 2>&1

  cd smalltalk/raspberry/
  echo "Building Smalltalk"
  make clean >> build_$1.log 2>&1
  make > build_$1.log 2>&1
 
  popd > /dev/null 2>&1
}

### building starts here

if [ "$1" = "clean" ]; then
   clean 1
   exit 0
fi

# Raspberry Pi 1/Zero
echo "Building Smalltalk-80 bare metal for Raspberry Pi 1/Zero"
build 1 
cp smalltalk/raspberry/kernel.img sdboot/
echo

# Raspberry Pi 2/3
echo "Building Smalltalk-80 bare metal for Raspberry Pi 2 and 3"
build 2 
cp smalltalk/raspberry/kernel7.img sdboot/
echo

# Raspberry Pi 4
echo "Building Smalltalk-80 bare metal for Raspberry Pi 4"
build 4
cp smalltalk/raspberry/kernel7l.img sdboot/
echo

echo "Finished"

