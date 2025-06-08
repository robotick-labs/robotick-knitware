cmake_minimum_required(VERSION 3.16)
project(barr_e)

# Path to the robotick-engine submodule
add_subdirectory(../../../robotick-engine cpp/robotick-engine-build)

add_executable(barr-e-main main.cpp)
target_link_libraries(barr-e-main PRIVATE robotick-engine)

# ====================================================================
# ✅ Symlink runtime data from robotick-engine → engine-data folder
# ====================================================================

set(ENGINE_DATA_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../robotick-engine/data")
set(ENGINE_DATA_DEST_DIR "${CMAKE_CURRENT_BINARY_DIR}/engine-data")

message(STATUS "🔗 Will symlink engine data from: ${ENGINE_DATA_SOURCE_DIR}")
message(STATUS "🔗            into binary path: ${ENGINE_DATA_DEST_DIR}")

# Only create the symlink if it doesn't already exist
if(NOT EXISTS "${ENGINE_DATA_DEST_DIR}")
    file(CREATE_LINK "${ENGINE_DATA_SOURCE_DIR}" "${ENGINE_DATA_DEST_DIR}" SYMBOLIC)
    message(STATUS "✅ Created symlink: ${ENGINE_DATA_DEST_DIR} → ${ENGINE_DATA_SOURCE_DIR}")
else()
    message(STATUS "♻️  Symlink or directory already exists: ${ENGINE_DATA_DEST_DIR}")
endif()

