/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"
#include "driver/gpio.h"

#define GPIO_INPUT_PIN_11 11
#define GPIO_INPUT_PIN_12 12
#define GPIO_OUTPUT_PIN_10 10

void app_main(void)
{
    gpio_config_t in_conf = {
        .pin_bit_mask = (1ULL << GPIO_INPUT_PIN_11) | (1ULL << GPIO_INPUT_PIN_12),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&in_conf);

    gpio_config_t out_conf = {
        .pin_bit_mask = (1ULL << GPIO_OUTPUT_PIN_10),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&out_conf);

    int toggle = 0;
    while (1)
    {
        gpio_set_level(GPIO_OUTPUT_PIN_10, toggle);
        toggle = !toggle;

        printf("GPIO 10: %d  GPIO 11: %d  GPIO 12: %d\n",
            gpio_get_level(GPIO_OUTPUT_PIN_10),
            gpio_get_level(GPIO_INPUT_PIN_11),
            gpio_get_level(GPIO_INPUT_PIN_12));
        vTaskDelay(4000 / portTICK_PERIOD_MS);

        printf("Hello world!\n");

        /* Print chip information */
        esp_chip_info_t chip_info;
        uint32_t flash_size;
        esp_chip_info(&chip_info);
        printf("This is %s chip with %d CPU core(s), %s%s%s%s, ",
            CONFIG_IDF_TARGET,
            chip_info.cores,
            (chip_info.features & CHIP_FEATURE_WIFI_BGN) ? "WiFi/" : "",
            (chip_info.features & CHIP_FEATURE_BT) ? "BT" : "",
            (chip_info.features & CHIP_FEATURE_BLE) ? "BLE" : "",
            (chip_info.features & CHIP_FEATURE_IEEE802154) ? ", 802.15.4 (Zigbee/Thread)" : "");

        unsigned major_rev = chip_info.revision / 100;
        unsigned minor_rev = chip_info.revision % 100;
        printf("silicon revision v%d.%d, ", major_rev, minor_rev);
        if(esp_flash_get_size(NULL, &flash_size) != ESP_OK) {
            printf("Get flash size failed");
            return;
        }

        printf("%" PRIu32 "MB %s flash\n", flash_size / (uint32_t)(1024 * 1024),
            (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

        printf("Minimum free heap size: %" PRIu32 " bytes\n", esp_get_minimum_free_heap_size());

        vTaskDelay(1000 / portTICK_PERIOD_MS);
        fflush(stdout);
    }
}
