/*
 * File:        touchSensor.h
 * Author:      Alex Doury
 * Created:     January 2024
 * 
 * Description: 
 * This header file provides the functions required to initialize and run the tasks 
 * for the touch sensor module.
 */
#pragma once

/**
 * @brief Initialize the touch sensor module.
 */
void touchSensor_init(void);

/**
 * @brief Task to read values sensed on all available touch pads. Print out values of all touch pads. 
 * @param pvParameter 
 */
void touchSensor_readTask(void *pvParameter);

