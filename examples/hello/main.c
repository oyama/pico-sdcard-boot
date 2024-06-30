/*
 * Copyright 2024, Hiroyuki OYAMA. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <stdio.h>
#include <pico/stdlib.h>

int main(void) {
    stdio_init_all();

    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    while (true) {
        printf("Hello World! 1\n");
        gpio_put(LED_PIN, 1);
        sleep_ms(250);

        printf("Hello World! 0\n");
        gpio_put(LED_PIN, 0);
        sleep_ms(250);
    }
}
