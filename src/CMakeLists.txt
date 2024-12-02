# This file was automatically generated for projects
# without default 'CMakeLists.txt' file.

# .c files
FILE(GLOB_RECURSE app_sources
        ${CMAKE_SOURCE_DIR}/src/*.c*/
        ${CMAKE_SOURCE_DIR}/src/{console,drivers,filesystem}/*.c*
)

# .h file's directories
idf_component_register(SRCS ${app_sources}
    INCLUDE_DIRS .       
        console
        drivers
        filesystem
)

# USB lib for console
set(EXTRA_COMPONENT_DIRS $ENV{IDF_PATH}/components/usb/host)
