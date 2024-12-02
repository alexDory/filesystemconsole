#ifndef EMMC08G_H
#define EMMC08G_H

#include <stdio.h>
#include "esp_err.h"
#include "esp_log.h"


/**
 * @brief  Initializes the eMMC module.
 * @return esp_err_t  Error code indicating success or failure.
 * @note   Initializes the eMMC module, mounts the filesystem, and prints card info.
 */
esp_err_t emmc_init(void);

/**
 * @brief Runs eMMC tests.
 * @note  Runs a series of eMMC tests, including writing / reading
 *        single and multiple blocks and erase.
 */
void emmc_run_tests(void);

#endif