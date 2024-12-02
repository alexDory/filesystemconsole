/*
 * File:        fileSystem.h
 * Author:      Alex Doury
 * Created:     December 2024
 * 
 * Description: 
 * This header file provides the functions required to build and control
 * the fileSystem on the eMMC.
 */

#pragma once 

/**
 * @brief   Initializes the partition and the fileSystem structure on the eMMC.
 */
void fileSystem_init();

/**
 * @brief   Create a directory in the current directory.
 * @param   path Name of the folder.
 */
void fileSystem_mkdir(const char* path);

/**
 * @brief   Remove a directory in the current directory.
 * @param   path Name of the folder.
 */
void fileSystem_rmdir(const char *path);

/**
 * @brief   Change the current directory to the given path. The full path from root is needed
 * @param   path Directory of destination. "/" for root.
 */
void fileSystem_cd(const char* path);

/**
 * @brief   Display the current directory path.
 */
void fileSystem_pwd();

/**
 * @brief   List all files in current directory.
 */
void fileSystem_ls();

/**
 * @brief   Prints the content of a file to the console.
 * @param   path path of file.
 */
void fileSystem_printFileContent(const char *filepath);