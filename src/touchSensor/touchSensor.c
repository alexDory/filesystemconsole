/*
 * File:        touchSensor.c
 * Author:      Alex Doury
 * Created:     January 2024
 * 
 * Description: 
 * This source file provides the functions required to initialize and run the tasks 
 * for the touch sensor module.
 */

#include "esp_log.h"
#include "driver/touch_sensor.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>
#include <inttypes.h>
#include "driver/touch_pad.h"

#define TOUCH_THRESH_MODE    (1)

/*==================================*/
/*============= PUBLIC =============*/
/*==================================*/
void touchSensor_init(void)
{
    ESP_ERROR_CHECK(touch_pad_init());
    ESP_ERROR_CHECK(touch_pad_set_voltage(TOUCH_HVOLT_2V7, TOUCH_LVOLT_0V5, TOUCH_HVOLT_ATTEN_1V));

    for (int i = 1; i < TOUCH_PAD_MAX; i++) {
        ESP_ERROR_CHECK(touch_pad_config(i));
        ESP_ERROR_CHECK(touch_pad_set_thresh(i, TOUCH_THRESH_MODE));
    }

    touch_filter_config_t filter_info = {
        .mode = TOUCH_PAD_FILTER_IIR_16,
        .debounce_cnt = 1,
        .noise_thr = 0,
        .jitter_step = 4,
        .smh_lvl = TOUCH_PAD_SMOOTH_IIR_2,
    };

    ESP_ERROR_CHECK(touch_pad_filter_set_config(&filter_info));
    ESP_ERROR_CHECK(touch_pad_filter_enable());
    ESP_ERROR_CHECK(touch_pad_fsm_start());
}

void touchSensor_readTask(void *pvParameter)
{
    uint32_t touch_value;

    while (1) 
    {
        for (int i = 1; i < TOUCH_PAD_MAX; i++) 
        {
            ESP_ERROR_CHECK(touch_pad_read_raw_data(i, &touch_value));
            printf("T%d : [%lu] ", i, touch_value);
            vTaskDelay(2 / portTICK_PERIOD_MS);
        }
        printf("\n");
        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}

