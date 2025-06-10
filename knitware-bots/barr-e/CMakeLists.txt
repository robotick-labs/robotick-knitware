cmake_minimum_required(VERSION 3.21)
project(barr-e LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add robotick-engine from local path
add_subdirectory(../../robotick-engine/cpp ${CMAKE_BINARY_DIR}/robotick-engine)

# Add only brain side in this CMake tree (body is built via ESP-IDF separately)
add_subdirectory(brain)
