cmake_minimum_required(VERSION 3.21)
project(robotick-knitware)

set(ROBOTICK_BOT "" CACHE STRING "Which bot to build (e.g. barr-e, magg-e)")

if(NOT ROBOTICK_BOT)
  message(FATAL_ERROR "You must set ROBOTICK_BOT to one of the subdirectories.")
endif()

add_subdirectory(${ROBOTICK_BOT})
