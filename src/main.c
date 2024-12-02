#include "client.h"
#include "filesystem.h"
#include "esp_log.h"

static const char *TAG = "app_main";

#ifndef PIO_UNIT_TESTING

void app_main(void) 
{
    ESP_LOGI(TAG,"app_main starting !");

    fileSystem_init();
    CLI_console_init();
}
#endif