cmake_minimum_required(VERSION 3.23)
project(clench VERSION 0.1.0)

if((${CMAKE_SYSTEM_PROCESSOR} STREQUAL "i386") OR
    (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "AMD64") OR
    (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64"))
    enable_language(ASM_NASM)
endif()

include_directories(${PROJECT_SOURCE_DIR})

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")

include(${PROJECT_SOURCE_DIR}/cmake/ClenchBuild.cmake)

add_subdirectory("clench")
add_subdirectory("editor")
