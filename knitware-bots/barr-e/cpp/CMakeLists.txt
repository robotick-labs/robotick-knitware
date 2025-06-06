cmake_minimum_required(VERSION 3.16)
project(barr_e)

# Path to the robotick-engine submodule
add_subdirectory(../../../robotick-engine cpp/robotick-engine-build)

add_executable(barr-e-main main.cpp)
target_link_libraries(barr-e-main PRIVATE robotick-engine)
