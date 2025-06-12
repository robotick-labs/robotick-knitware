# Copyright Robotick Labs
# SPDX-License-Identifier: Apache-2.0

# /CMakeLists.txt

cmake_minimum_required(VERSION 3.21)
project(robotick-knitware LANGUAGES C CXX)

# Always add the engine first so it can be linked by downstream targets
add_subdirectory(robotick-engine/cpp)

# Add the barr-e brain executable (builds from its own subdir)
add_subdirectory(knitware-bots/barr-e/brain)
