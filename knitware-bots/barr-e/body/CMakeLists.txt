# knitware-bots/barr-e/CMakeLists.txt

cmake_minimum_required(VERSION 3.16)

# Detect ESP32 build based on IDF_PATH
if(DEFINED ENV{IDF_PATH})
    set(ROBOTICK_PLATFORM_ESP32 ON CACHE BOOL "Platform is ESP32")
endif()

# Apply workload preset if passed via environment
if(DEFINED ENV{WORKLOAD_PRESET})
    set(WORKLOAD_PRESET $ENV{WORKLOAD_PRESET} CACHE STRING "Workload preset")
endif()

# --------------------------------------
# ESP32 build (via ESP-IDF)
# --------------------------------------
if(ROBOTICK_PLATFORM_ESP32)
    message(STATUS "🧠 Detected ESP32 build environment")

    set(EXTRA_COMPONENT_DIRS
        ${CMAKE_SOURCE_DIR}/../../robotick-engine/platforms/esp32/components
    )

    include($ENV{IDF_PATH}/tools/cmake/project.cmake)
    project(barr-e)
endif()

# --------------------------------------
# Ubuntu / Desktop build
# --------------------------------------
if(NOT ROBOTICK_PLATFORM_ESP32)
    project(barr-e)  # ✅ Only for desktop builds!
    add_executable(barr-e main/main.cpp)

    target_include_directories(barr-e PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../robotick-engine/cpp/include")
    target_link_libraries(barr-e PRIVATE robotick-engine)

    # 📂 Create symlink to engine runtime data
    set(ENGINE_DATA_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../robotick-engine/data")
    set(ENGINE_DATA_DEST_DIR "${CMAKE_CURRENT_BINARY_DIR}/engine-data")

    if(NOT EXISTS "${ENGINE_DATA_DEST_DIR}")
        file(CREATE_LINK "${ENGINE_DATA_SOURCE_DIR}" "${ENGINE_DATA_DEST_DIR}" SYMBOLIC)
        message(STATUS "✅ Created symlink: ${ENGINE_DATA_DEST_DIR} → ${ENGINE_DATA_SOURCE_DIR}")
    else()
        message(STATUS "♻️  Symlink or directory already exists: ${ENGINE_DATA_DEST_DIR}")
    endif()
endif()
