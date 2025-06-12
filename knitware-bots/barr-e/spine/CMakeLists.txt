# /knitware-bots/barr-e/spine/CMakeLists.txt

cmake_minimum_required(VERSION 3.16)

set(EXTRA_COMPONENT_DIRS
    ../../../robotick-engine/platforms/esp32/components
)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(barr-e-spine)
