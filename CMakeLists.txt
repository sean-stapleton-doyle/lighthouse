
cmake_minimum_required(VERSION 3.20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PICO_SDK_PATH "${CMAKE_CURRENT_SOURCE_DIR}/libs/pico-sdk")

include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(lighthouses C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()


add_executable(lighthouses
    src/main.cpp
    src/fade_controller.cpp
    src/tlc5947.cpp
)

target_include_directories(lighthouses PRIVATE include)

pico_add_extra_outputs(lighthouses)

pico_enable_stdio_uart(lighthouses 1)

target_link_libraries(lighthouses pico_stdlib)
