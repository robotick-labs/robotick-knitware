#!/bin/bash
set -e

clear

# 💣 Delete old build folder
rm -rf build

export WORKLOAD_PRESET=esp32s3-workloads-barr-e

echo "🚀 Building & flashing…"
idf.py build
# idf.py -p /dev/ttyUSB0 flash monitor
