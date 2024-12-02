/*
 * File:        fileSystem.c
 * Author:      Alex Doury
 * Created:     October 2024
 * 
 * Description: 
 * This source file provides the functions required to build and control
 * the fileSystem on the eMMC.
 */

#include "fileSystem.h"
#include <unistd.h> 
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <dirent.h>
#include "sdmmc_cmd.h"
#include "esp_vfs_fat.h"
#include "driver/sdmmc_host.h"
#include "emmc08g.h"

#define EMMC_MAX_RETRIES    3
#define EMMC_RETRY_DELAY_MS 100
#define MAX_PATH_LENGTH 256

static const char *TAG = "Filesystem";


static char current_dir[MAX_PATH_LENGTH] = "/emmc";

/* Initializing the filesystem with some basics. */
static const char *fsConf[] = { "/emmc/Documents", "/emmc/Documents/WonderfullFile",
                                 "/emmc/Download", "/emmc/Config"};

/*==================================*/
/*============= PUBLIC =============*/
/*==================================*/

void fileSystem_init() 
{
    emmc_init();

    int numDirectories = sizeof(fsConf) / sizeof(fsConf[0]);
    for (int i = 0; i < numDirectories; i++) {
        ESP_LOGI(TAG, "Creating directory: %s", fsConf[i]);
        fileSystem_mkdir(fsConf[i]);
    }

    FILE *file = fopen("/emmc/verylongpathtest.txt", "w");
    if (file) {
        ESP_LOGI(TAG, "Filesystem accessible at /emmc.");
        fclose(file);
    } else {
        ESP_LOGE(TAG, "Filesystem not accessible at /emmc.");
        return;
    }
}

void fileSystem_mkdir(const char *path) 
{
    int ret;
    for (int retry = 0; retry < EMMC_MAX_RETRIES; retry++) {
        ret = mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

        if (ret == 0) {
            ESP_LOGI(TAG, "Directory created: %s", path);
            return;  
        } 
        else if (errno == EEXIST) {
            ESP_LOGE(TAG, "Directory already exists: %s", path);
            return;  
        } 
        else {
            ESP_LOGE(TAG, "Attempt %d: Error creating directory %s, error code: %d", retry + 1, path, errno);
        }

        vTaskDelay(pdMS_TO_TICKS(EMMC_RETRY_DELAY_MS));  
    }
    
    ESP_LOGE(TAG, "Failed to create directory %s after %d attempts", path, EMMC_MAX_RETRIES);
}

void fileSystem_rmdir(const char *path) 
{
    int ret;
    for (int retry = 0; retry < EMMC_MAX_RETRIES; retry++) {
        ret = rmdir(path);

        if (ret == 0) {
            ESP_LOGI(TAG, "Directory deleted: %s", path);
            return;  
        }else {
            ESP_LOGE(TAG, "Attempt %d: Error deleting directory %s, error code: %d", retry + 1, path, errno);
        }

        vTaskDelay(pdMS_TO_TICKS(EMMC_RETRY_DELAY_MS));  
    }
    
    ESP_LOGE(TAG, "Failed to delete directory %s after %d attempts", path, EMMC_MAX_RETRIES);
}

void fileSystem_cd(const char* path) 
{
    DIR* res = opendir(path);
    
    if (res != NULL) {
        strncpy(current_dir, path, MAX_PATH_LENGTH);
        ESP_LOGI(TAG, "Changed directory to: %s\n", current_dir);
    } else {
        ESP_LOGE(TAG, "Error: Failed to change directory to %s\n", path);
    }
}

void fileSystem_pwd() 
{
    ESP_LOGI(TAG, "Current dir : %s", current_dir);
}

void fileSystem_ls() 
{
    struct dirent* entry;
    DIR* dir = opendir(current_dir);
    if (dir == NULL) 
    {
        ESP_LOGI(TAG, "Failed to open directory: %s\n", current_dir);
        return;
    }

    ESP_LOGI(TAG, "Contents of %s:\n", current_dir);
    while ((entry = readdir(dir)) != NULL) 
    {
        struct stat entry_stat;
        char path[MAX_PATH_LENGTH];
        int ret = snprintf(path, MAX_PATH_LENGTH - 1, "%s/%s", current_dir, entry->d_name);

        if (ret >= MAX_PATH_LENGTH - 1) 
        {
            ESP_LOGI(TAG, "Path too long to concatenate: %s\n", entry->d_name);
            continue;
        }

        if (stat(path, &entry_stat) == 0) 
        {
            if (S_ISDIR(entry_stat.st_mode)) {
                ESP_LOGI(TAG, "  [DIR]  %s\n", entry->d_name);
            } else if (S_ISREG(entry_stat.st_mode)) {
                ESP_LOGI(TAG, "  [FILE] %s\n", entry->d_name);
            } else {
                ESP_LOGI(TAG, "  [OTHER] %s\n", entry->d_name);
            }
        } else {
            ESP_LOGI(TAG, "  [UNKNOWN] %s\n", entry->d_name);
        }
    }
    closedir(dir);
}

void fileSystem_printFileContent(const char *filepath) 
{
    char full_path[MAX_PATH_LENGTH] = "";
    strcat(full_path,current_dir);
    strcat(full_path,"/");
    strcat(full_path,filepath);
    FILE *file = fopen(full_path, "r");

    if (file == NULL) {
        ESP_LOGE(TAG, "Failed to open file: %s", full_path);
        return;
    }

    char buffer[128];
    ESP_LOGI(TAG, "Content of file %s:", full_path);

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        printf("%s", buffer);
    }

    if (ferror(file)) {
        ESP_LOGE(TAG, "Error reading file: %s", full_path);
    }

    fclose(file);
}
