/*
 * Copyright 2024, Hiroyuki OYAMA. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <pico/stdlib.h>
#include <hardware/flash.h>
#include <hardware/sync.h>
#include "filesystem/vfs.h"

#if !defined(SD_BOOT_FLASH_OFFSET)
#define SD_BOOT_FLASH_OFFSET         (256 * 1024)
#endif

static bool is_same_existing_program(FILE *fp) {
    uint8_t buffer[FLASH_SECTOR_SIZE] = {0};
    size_t program_size = 0;
    size_t len = 0;
    while ((len = fread(buffer, 1, sizeof(buffer), fp)) > 0) {
        uint8_t *flash = (uint8_t *)(XIP_BASE + SD_BOOT_FLASH_OFFSET + program_size);
        if (memcmp(buffer, flash, len) != 0)
            return false;
        program_size += len;
    }
    return true;
}

static bool load_program(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("fopen %s failed: %s\n", filename, strerror(errno));
        return false;
    }
    if (is_same_existing_program(fp)) {
        printf("No update required, program is up to date.\n");
        return true;
    }

    printf("Updates found. Update programme\n");
    if (fseek(fp, 0, SEEK_SET) == -1) {
        printf("fseek failed: %s\n", strerror(errno));
        return false;
    }
    size_t program_size = 0;
    uint8_t buffer[FLASH_SECTOR_SIZE] = {0};
    size_t len = 0;
    while ((len = fread(buffer, 1, sizeof(buffer), fp)) > 0) {
        uint32_t ints = save_and_disable_interrupts();
        flash_range_erase(SD_BOOT_FLASH_OFFSET + program_size, FLASH_SECTOR_SIZE);
        flash_range_program(SD_BOOT_FLASH_OFFSET + program_size, buffer, len);
        restore_interrupts(ints);

        program_size += len;
    }
    printf("Program loaded successfully!\n");
    fclose(fp);
    return true;
}

void launch_application_from(uint32_t *app_location) {
    // https://vanhunteradams.com/Pico/Bootloader/Bootloader.html
    uint32_t *new_vector_table = app_location;
    volatile uint32_t *vtor = (uint32_t *)(PPB_BASE + M0PLUS_VTOR_OFFSET);
    *vtor = (uint32_t)new_vector_table;
    asm volatile (
        "msr msp, %0\n"
        "bx %1\n"
        :
        : "r" (new_vector_table[0]), "r" (new_vector_table[1])
        : );
}

int main() {
    stdio_init_all();
    fs_init();

    load_program("/sd/firmware.bin");
    launch_application_from((uint32_t *)(XIP_BASE + SD_BOOT_FLASH_OFFSET));
    while (1)
        tight_loop_contents();
}
