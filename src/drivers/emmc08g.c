#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "driver/sdmmc_host.h"
#include "driver/sdmmc_defs.h"
#include "esp_vfs_fat.h"

#define   EMMC_D0   GPIO_NUM_2
#define   EMMC_D1   GPIO_NUM_4
#define   EMMC_D2   GPIO_NUM_12
#define   EMMC_D3   GPIO_NUM_13
#define   EMMC_CLK  GPIO_NUM_14
#define   EMMC_CMD  GPIO_NUM_15
#define   EMMC_BUS_WIDTH      4

#define MOUNT_POINT "/emmc"
#define FORMAT_IF_MOUNT_FAILED true

static const char *TAG = "eMMC_Driver";
static SemaphoreHandle_t emmc_mutex = NULL;


static esp_err_t emmc_secure_erase(void);
static esp_err_t emmc_test_read_write(void);


/*===================================*/
/*============= PRIVATE =============*/
/*===================================*/


/**
 * @brief Unit test for writing and reading blocks on the eMMC card.
 */
static esp_err_t emmc_test_read_write(void) 
{
    const char *test_file = MOUNT_POINT "/test.txt";
    FILE *f = fopen(test_file, "w");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to open file for writing");
        return ESP_FAIL;
    }
    fprintf(f, "eMMC Test: Write successful!");
    fclose(f);
    ESP_LOGI(TAG, "File written");

    f = fopen(test_file, "r");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to open file for reading");
        return ESP_FAIL;
    }
    char buffer[64];
    fgets(buffer, sizeof(buffer), f);
    fclose(f);
    ESP_LOGI(TAG, "File read: %s", buffer);

    return ESP_OK;
}

static esp_err_t emmc_secure_erase(void) 
{
    ESP_LOGI(TAG, "Secure erase is not implemented in this example.");
    return ESP_OK;
}

/*==================================*/
/*============= PUBLIC =============*/
/*==================================*/

esp_err_t emmc_init(void) 
{
    esp_err_t ret;

    /* Creating mutex for safety */
    emmc_mutex = xSemaphoreCreateMutex();
    if (emmc_mutex == NULL) {
        ESP_LOGE(TAG, "Failed to create mutex");
        return ESP_ERR_NO_MEM;
    }

    /* Initializing hosts */
    sdmmc_host_t host = SDMMC_HOST_DEFAULT();
    host.flags = SDMMC_HOST_FLAG_4BIT;
    host.max_freq_khz = SDMMC_FREQ_DEFAULT;

    /* Initializing slots */
    sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT();
    slot_config.width = EMMC_BUS_WIDTH;
    slot_config.flags |= SDMMC_SLOT_FLAG_INTERNAL_PULLUP;
    slot_config.clk = EMMC_CLK;
    slot_config.cmd = EMMC_CMD;
    slot_config.d0 = EMMC_D0;
    slot_config.d1 = EMMC_D1;
    slot_config.d2 = EMMC_D2;
    slot_config.d3 = EMMC_D3;

    vTaskDelay(pdMS_TO_TICKS(100));

    /* Mount eMMC */
    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = true,
        .max_files = 20,
        .allocation_unit_size = 16 * 1024,
        .disk_status_check_enable = true
    };

    sdmmc_card_t *card = NULL;
    ESP_LOGI(TAG, "Mounting filesystem...");
    ret = esp_vfs_fat_sdmmc_mount(MOUNT_POINT, &host, &slot_config, &mount_config, &card);

    if (ret != ESP_OK) {
        vSemaphoreDelete(emmc_mutex);
        emmc_mutex = NULL;
        ESP_LOGE(TAG, "Failed to mount filesystem");
        sdmmc_host_deinit();
        return ret;
    }

    ESP_LOGI(TAG, "Filesystem mounted");

    return ESP_OK;
}

void emmc_run_tests(void) 
{
    ESP_LOGI(TAG, "Starting eMMC driver embedded test session");

    esp_err_t ret = emmc_test_read_write();

    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Read/Write test failed");
    }

    ret = emmc_secure_erase();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Secure erase failed");
    }

    ESP_LOGI(TAG, "eMMC driver completed");
}