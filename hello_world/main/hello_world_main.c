/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

// Motor IN1 outputs (speed/enable)
#define MOTOR1_IN1  8
#define MOTOR2_IN1  9
#define MOTOR3_IN1  45

// Motor IN2 outputs (direction)
#define DIR1        11   // Motor 2 IN2
#define DIR2        10   // Motor 3 IN2
#define DIR3        46   // Motor 1 IN2

// LED output
#define LED         44

// Grip output
#define GRIP        18

// IPRORI inputs
#define IPRORI_1    1
#define IPRORI_2    2
#define IPRORI_3    3

// N_FAULT inputs (active low)
#define NFAULT_1    12
#define NFAULT_2    13
#define NFAULT_3    14

#define CYCLE_MS    2000
#define GRIP_MS     3000

static void print_status(int cycle, const char *state)
{
    printf("\n--- Cycle %d | %-3s ---\n", cycle, state);
    printf("  Motors  : M1_IN1=%-1d  M2_IN1=%-1d  M3_IN1=%-1d\n",
        gpio_get_level(MOTOR1_IN1),
        gpio_get_level(MOTOR2_IN1),
        gpio_get_level(MOTOR3_IN1));
    printf("  Dir     : DIR1=%-1d  DIR2=%-1d  DIR3=%-1d\n",
        gpio_get_level(DIR1),
        gpio_get_level(DIR2),
        gpio_get_level(DIR3));
    printf("  IPRORI  : 1=%-1d  2=%-1d  3=%-1d\n",
        gpio_get_level(IPRORI_1),
        gpio_get_level(IPRORI_2),
        gpio_get_level(IPRORI_3));
    printf("  N_FAULT : 1=%-1d  2=%-1d  3=%-1d  %s\n",
        gpio_get_level(NFAULT_1),
        gpio_get_level(NFAULT_2),
        gpio_get_level(NFAULT_3),
        (!gpio_get_level(NFAULT_1) || !gpio_get_level(NFAULT_2) || !gpio_get_level(NFAULT_3))
            ? "<<< FAULT" : "OK");
}

static void grip_task(void *arg)
{
    for (;;) {
        gpio_set_level(GRIP, 1);
        vTaskDelay(GRIP_MS / portTICK_PERIOD_MS);
        gpio_set_level(GRIP, 0);
        vTaskDelay(GRIP_MS / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    gpio_config_t in_conf = {
        .pin_bit_mask = (1ULL << IPRORI_1) | (1ULL << IPRORI_2) | (1ULL << IPRORI_3) |
                        (1ULL << NFAULT_1) | (1ULL << NFAULT_2) | (1ULL << NFAULT_3),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&in_conf);

    gpio_config_t out_conf = {
        .pin_bit_mask = (1ULL << MOTOR1_IN1) | (1ULL << MOTOR2_IN1) | (1ULL << MOTOR3_IN1) |
                        (1ULL << DIR1) | (1ULL << DIR2) | (1ULL << DIR3) | (1ULL << LED) | (1ULL << GRIP),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&out_conf);

    xTaskCreate(grip_task, "grip", 2048, NULL, 5, NULL);

    for (int i = 1; ; i++) {
        int dir = i % 2;

        gpio_set_level(MOTOR1_IN1, 1);
        gpio_set_level(MOTOR2_IN1, 1);
        gpio_set_level(MOTOR3_IN1, 1);
        gpio_set_level(DIR1,       dir);
        gpio_set_level(DIR2,       dir);
        gpio_set_level(DIR3,       dir);
        gpio_set_level(LED,        1);
        print_status(i, "ON");
        vTaskDelay(CYCLE_MS / portTICK_PERIOD_MS);

        gpio_set_level(MOTOR1_IN1, 0);
        gpio_set_level(MOTOR2_IN1, 0);
        gpio_set_level(MOTOR3_IN1, 0);
        gpio_set_level(DIR1,       0);
        gpio_set_level(DIR2,       0);
        gpio_set_level(DIR3,       0);
        gpio_set_level(LED,        0);
        print_status(i, "OFF");
        vTaskDelay(CYCLE_MS / portTICK_PERIOD_MS);
    }
}
