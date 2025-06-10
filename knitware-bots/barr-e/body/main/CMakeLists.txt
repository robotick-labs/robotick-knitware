# knitware-bots/barr-e/main/CMakeLists.txt

idf_component_register(
    SRCS "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
    INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
)

# Optional: if your main.cpp uses robotick headers directly
target_compile_definitions(${COMPONENT_LIB} PRIVATE ROBOTICK_PLATFORM_ESP32)
