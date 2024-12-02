#include <unity.h>
#include <stdio.h>
#include "esp_err.h"
#include "driver/sdmmc_host.h"
#include "driver/sdmmc_defs.h"
#include "esp_vfs_fat.h"

#define MOUNT_POINT "/emmc"

sdmmc_card_t *card;

// Helper function to initialize eMMC
esp_err_t emmc_init(void) {
    sdmmc_host_t host = SDMMC_HOST_DEFAULT();
    sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT();
    slot_config.width = 4; 
    slot_config.flags |= SDMMC_SLOT_FLAG_INTERNAL_PULLUP;

    esp_vfs_fat_mount_config_t mount_config = {
        .format_if_mount_failed = true,
        .max_files = 5,
    };

    return esp_vfs_fat_sdmmc_mount(MOUNT_POINT, &host, &slot_config, &mount_config, &card);
}

// Test Initialization
void test_emmc_init(void) {
    esp_err_t ret = emmc_init();
    TEST_ASSERT_EQUAL_INT(ESP_OK, ret);
    TEST_ASSERT_NOT_NULL(card);
}

// Test Read and Write
void test_emmc_read_write(void) {
    const char *test_file = MOUNT_POINT "/test.txt";

    // Write to the file
    FILE *f = fopen(test_file, "w");
    TEST_ASSERT_NOT_NULL(f);
    fprintf(f, "eMMC Test: Write successful!");
    fclose(f);

    // Read back from the file
    f = fopen(test_file, "r");
    TEST_ASSERT_NOT_NULL(f);
    char buffer[64] = {0};
    fgets(buffer, sizeof(buffer), f);
    fclose(f);

    TEST_ASSERT_EQUAL_STRING("eMMC Test: Write successful!", buffer);
}

// Test Cleanup (Optional)
void test_emmc_cleanup(void) {
    remove(MOUNT_POINT "/test.txt");
}

void test_emmc_unmount(void) {
    esp_err_t ret = esp_vfs_fat_sdmmc_unmount();
    TEST_ASSERT_EQUAL_INT(ESP_OK, ret);
}

// Main Unity Test Runner
void app_main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_emmc_init);
    RUN_TEST(test_emmc_read_write);
    RUN_TEST(test_emmc_cleanup);
    RUN_TEST(test_emmc_unmount);
    UNITY_END();
}
