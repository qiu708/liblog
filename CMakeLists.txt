cmake_minimum_required(VERSION 3.20)
project(liblog C CXX)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()
set(CMAKE_CXX_STANDARD 17)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/build_bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/build_lib)

#include_directories()
include_directories(${PROJECT_SOURCE_DIR}
        ${PROJECT_BINARY_DIR}
        lib/libconfig
        )
if(NOT TARGET libconfig)
    add_subdirectory(lib/libconfig)
endif()
add_subdirectory(liblog)
