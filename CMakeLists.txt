cmake_minimum_required(VERSION 3.13...3.27)
include(vendor/pico_sdk_import.cmake)
add_subdirectory(vendor/pico-vfs)

project(pico-sd-boot C CXX ASM)
set(FAMILY rp2040)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

add_executable(sdboot
  main.c
  fs_init.c
)
target_link_libraries(sdboot PRIVATE
  pico_stdlib
  hardware_flash
  hardware_sync
  blockdevice_sd
  filesystem_fat
  filesystem_vfs
)
target_link_options(sdboot PRIVATE -Wl,--print-memory-usage)
pico_enable_stdio_usb(sdboot 1)
pico_add_extra_outputs(sdboot)


function(enable_sdcard_app target)
  pico_set_linker_script(${target} ${CMAKE_SOURCE_DIR}/memmap_sdcard_app.ld)
endfunction()

add_subdirectory(examples EXCLUDE_FROM_ALL)
