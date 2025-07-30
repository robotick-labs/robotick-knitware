# knitware-bots/barr-e/spine/main/CMakeLists.txt

idf_component_register(
    SRCS "spine_main.cpp"
    INCLUDE_DIRS "."
    PRIV_REQUIRES robotick-engine
)

