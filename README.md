# Raspberry Pi SD Card Bootloader

`pico-sdcard-boot` is a custom bootloader for the Raspberry Pi Pico. This bootloader provides the functionality to load and execute applications from an SD card, designed to enable developers to deploy programs to the Pico using an SD card easily.

## Bootloader Build and Installation

Clone the source code and initialize the submodules.

```bash
git clone https://github.com/oyama/pico-sdcard-boot.git
cd pico-sdcard-boot
git submodule update --init --recursive
```

Build the bootloader.

```bash
mkdir build; cd build
PICO_SDK_PATH=/path/to/pico-sdk cmake ..
make
```
Once the build is complete, drop the generated sd-boot.uf2 firmware onto the Pico mounted in BOOTSEL mode to write it.

## Application Build and Deployment on SD Card

This section explains how to build and deploy applications on an SD card. Below is a simple example of a CMakeLists.txt for an application.

```CMakeLists.txt
cmake_minimum_required(VERSION 3.13...3.27)
include(vendor/pico_sdk_import.cmake)
add_subdirectory(pico-sdcard-boot)

project(hello)
set(FAMILY rp2040)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

add_executable(hello main.c)
target_link_libraries(hello PRIVATE pico_stdlib)
pico_enable_stdio_usb(hello 1)
pico_add_extra_outputs(hello)

enable_sdcard_app(hello)
```
The `enable_sdcard_app()` function sets the necessary linker script for projects that boot from an SD card.

### Build and Deployment Process

1. Build the project using the above CMakeLists.txt.

```bash
mkdir build; cd build
PICO_SDK_PATH=/path/to/pico-sdk cmake ..
make
```

2. Once the build is complete, copy the generated `hello.bin` file to the root directory of the SD card as `firmware.bin`.
3. Insert the SD card into the Pico and reset the device; the new application will automatically load and run.

## License

This project is licensed under the 3-Clause BSD License. For details, see the [LICENSE](LICENSE.md) file.
