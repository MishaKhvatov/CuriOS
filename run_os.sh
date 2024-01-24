#!/bin/bash

# Stop the script if any command fails
set -e

# Clean the build environment
make clean

# Build the project
./build.sh

# Change directory to bin
cd bin

# Run the OS image in QEMU
qemu-system-i386  -hda ./os.bin

# Return to the original directory
cd ..
