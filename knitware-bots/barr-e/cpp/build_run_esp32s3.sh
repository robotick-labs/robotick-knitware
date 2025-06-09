#!/bin/bash
set -e

# Configure the build (runs only if not already configured)
cmake --preset barr-e-esp32s3-debug

# Build and flash
cmake --build --preset barr-e-esp32s3-debug

# Flash and monitor the device
idf.py -p /dev/ttyUSB0 flash monitor
