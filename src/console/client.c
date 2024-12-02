/*
 * File:        client.c
 * Author:      Alex Doury
 * Created:     Decembre 2024
 * 
 * Description: 
 * This source file provides the functions required to initialize and run the console
 * through the USB/JTAG channel.
 */

#include "esp_console.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "client.h"
#include "commands.h"

typedef struct {
    const char* command;
    const char* help;
    esp_console_cmd_func_t func;
} cmd_t;

static const char *TAG = "Console";

static void CLI_register_commands(void);

static const cmd_t custom_commands[] = {
    { "reboot",                        "Reboot the ESP32 device",                                                               esp_restart                    },
    { "mkdir",                         "Create new directory",                                                                  console_mkdir                  },
    { "cd",                            "Change directory",                                                                      console_cd                     },
    { "ls",                            "List all directories in current directory",                                             console_ls                     },
    { "pwd",                           "Display the path to current directory",                                                 console_pwd                    },
    { "rmdir",                         "Remove the specified directory",                                                        console_rmdir                  },
    { "printfile",                     "Print the content of a file, path is relative to current directory",                    console_printFileContent       },
    { "freeheap",                      "Print the free heap size in RAM memory",                                                console_freeHeap               },
    { "testemmc",                      "Run the test sequence from the emmc08g custom driver",                                  console_testemmc               },

};


/*===================================*/
/*============= PRIVATE =============*/
/*===================================*/

/**
 * @brief  Setup and register the basic custom commands.
 */
static void client_register_commands(void)
{
    for (size_t i = 0; i < sizeof(custom_commands) / sizeof(custom_commands[0]); i++) {
        const cmd_t* cmd = &custom_commands[i];

        esp_console_cmd_t esp_cmd = {
            .command = cmd->command,
            .help = cmd->help,
            .hint = NULL,
            .func = cmd->func,
            .argtable = NULL
        };

        ESP_ERROR_CHECK(esp_console_cmd_register(&esp_cmd));
    }

    ESP_LOGI(TAG, "Custom commands registered.");
}

/*==================================*/
/*============= PUBLIC =============*/
/*==================================*/

void client_console_init(void)
{
    esp_console_repl_t *reply = NULL;
    esp_console_repl_config_t reply_config = ESP_CONSOLE_REPL_CONFIG_DEFAULT();

    reply_config.prompt = "fancyConsole>";
    reply_config.max_cmdline_length = 256;

    client_register_commands();

    /* Register commands */
    esp_console_register_help_command();
    esp_console_dev_usb_serial_jtag_config_t hw_config = ESP_CONSOLE_DEV_USB_SERIAL_JTAG_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_console_new_repl_usb_serial_jtag(&hw_config, &reply_config, &reply));
    ESP_ERROR_CHECK(esp_console_start_repl(reply));
}