/*
 * File:        commands.h
 * Author:      Alex Doury
 * Created:     Decembre 2024
 * 
 * Description: 
 * This header file provides the functions required to setup and execute 
 * the commands in the console.
 */
#pragma once

/**
 * @brief Create a folder in the specified folder.
 * @param argc  Number of commands inputs.
 * @param argv  Pointer array of the commands inputs.
 * @return 0 if success and 1 if fail.
 */
int console_mkdir(int argc, char **argv);

/**
 * @brief   List all files in current directory.
 * @param argc  Number of commands inputs.
 * @param argv  Pointer array of the commands inputs.
 * @return 0 if success and 1 if fail.
 */
int console_ls(int argc, char **argv);

/**
 * @brief   Change the current directory to the given path. 
 * The full path from root is needed
 * @param argc  Number of commands inputs.
 * @param argv  Pointer array of the commands inputs.
 * @return 0 if success and 1 if fail.
 */
int console_cd(int argc, char **argv);

/**
 * @brief   Display the current directory path.
 * @param argc  Number of commands inputs.
 * @param argv  Pointer array of the commands inputs.
 * @return 0 if success and 1 if fail.
 */
int console_pwd(int argc, char **argv);

/**
 * @brief Remove the specified directory/file.
 * @param argc  Number of commands inputs.
 * @param argv  Pointer array of the commands inputs.
 * @return 0 if success and 1 if fail.
 */
int console_rmdir(int argc, char **argv);

/**
 * @brief Remove the specified directory/file.
 * @param argc  Number of commands inputs.
 * @param argv  Pointer array of the commands inputs.
 * @return 0 if success and 1 if fail.
 */
int console_printFileContent(int argc, char **argv);

/**
 * @brief Print the free heap size in RAM memory.
 * @param argc  Number of commands inputs.
 * @param argv  Pointer array of the commands inputs.
 * @return 0 if success and 1 if fail.
 */
int console_freeHeap(int argc, char **argv);

/**
 * @brief Run the eMMC driver's test sequence.
 * @param argc  Number of commands inputs.
 * @param argv  Pointer array of the commands inputs.
 * @return 0 if success and 1 if fail.
 */
int console_testemmc(int argc, char **argv);