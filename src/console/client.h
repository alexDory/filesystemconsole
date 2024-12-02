/*
 * File:        client.h
 * Author:      Alex Doury
 * Created:     November 2024
 * 
 * Description: 
 * This header file provides the functions required to initialize and run the console
 * through the USB/JTAG channel.
 */
#pragma once

/**
 * @brief Initialize the esp_console system.
 */
void CLI_console_init(void);