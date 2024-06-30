cmake_minimum_required(VERSION 3.13)
project(hello)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_executable(hello main.c)
target_link_libraries(hello PRIVATE pico_stdlib)

pico_enable_stdio_usb(hello 1)
pico_add_extra_outputs(hello)
enable_sdcard_app(hello)
