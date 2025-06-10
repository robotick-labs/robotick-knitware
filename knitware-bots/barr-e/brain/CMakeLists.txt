cmake_minimum_required(VERSION 3.21)

# Optional workload preset (used in your build system)
if(DEFINED ENV{WORKLOAD_PRESET})
    set(WORKLOAD_PRESET $ENV{WORKLOAD_PRESET} CACHE STRING "Workload preset")
endif()

add_executable(barr-e-brain main.cpp)

target_link_libraries(barr-e-brain PRIVATE robotick-engine)

# Add shared folder for things like model.hpp
target_include_directories(barr-e-brain PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../shared
)

# Optional: symlink engine runtime data for testing
set(ENGINE_DATA_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../robotick-engine/data")
set(ENGINE_DATA_DEST_DIR "${CMAKE_CURRENT_BINARY_DIR}/engine-data")

message(STATUS "🔗 Will symlink engine data from: ${ENGINE_DATA_SOURCE_DIR}")
message(STATUS "🔗            into binary path: ${ENGINE_DATA_DEST_DIR}")

if(NOT EXISTS "${ENGINE_DATA_DEST_DIR}")
    file(CREATE_LINK "${ENGINE_DATA_SOURCE_DIR}" "${ENGINE_DATA_DEST_DIR}" SYMBOLIC)
    message(STATUS "✅ Created symlink: ${ENGINE_DATA_DEST_DIR} → ${ENGINE_DATA_SOURCE_DIR}")
else()
    message(STATUS "♻️  Symlink or directory already exists: ${ENGINE_DATA_DEST_DIR}")
endif()
