/*
 * File:        commands.c
 * Author:      Alex Doury
 * Created:     November 2024
 * 
 * Description: 
 * This source file provides the functions required to setup and execute 
 * the commands in the console.
 */

#include <stdio.h>
#include "esp_system.h"
#include "commands.h"
#include "filesystem.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "string.h"
#include "emmc08g.h"


#define ONE_SECOND                   1000/portTICK_PERIOD_MS

static const char *TAG = "Console";

/*==================================*/
/*============= PUBLIC =============*/
/*==================================*/

int console_mkdir(int argc, char **argv)
{
    if (argc > 2) {
        printf("Usage: mkdir <full_path>\n");
        return 1;
    }
    fileSystem_mkdir(argv[1]);
    return 0;
}

int console_ls(int argc, char **argv)
{
    if (argc > 1) {
        printf("Usage: ls\n");
        return 1;
    }
    fileSystem_ls();
    return 0;
}

int console_cd(int argc, char **argv)
{
    if(argc > 2){
        printf("Usage: cd <path>\n");
        return 1;
    }
    fileSystem_cd(argv[1]);
    return 0;
}

int console_pwd(int argc, char **argv)
{
    if(argc > 1){
        printf("Usage: pwd\n");
        return 1;
    }
    fileSystem_pwd();
    return 0;
}

int console_rmdir(int argc, char **argv)
{
    if(argc > 2){
        printf("Usage: rmdir <path>\n");
        return 1;
    }
    fileSystem_rmdir(argv[1]);
    return 0;
}

int console_printFileContent(int argc, char **argv)
{
    if(argc > 2)
    {
        printf("Usage: printfile <path>, remember to check current directory with pwd\n");
        return 1;
    }
    fileSystem_printFileContent(argv[1]);
    return 0;
}

int console_freeHeap(int argc, char **argv)
{
    if(argc > 1){
        printf("Usage: freeheap\n");
        return 1;
    }
    printf("Free heap size in RAM : %i bytes\n", (int) esp_get_free_heap_size());
    return 0;
}

int console_testemmc(int argc, char **argv)
{
    if(argc > 1){
        printf("Usage: testemmc\n");
        return 1;
    }
    emmc_run_tests();
    return 0;
}