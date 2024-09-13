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
add_subdirectory("engine")
add_subdirectory("editor")
add_subdirectory("modules")

get_property(MODULE_SO_PATH_LIST GLOBAL PROPERTY _CLENCH_MODULE_SO_PATH_LIST)
get_property(MODULE_NAME_LIST GLOBAL PROPERTY _CLENCH_MODULE_NAME_LIST)

message("User module shared object list: ${MODULE_SO_PATH_LIST}")

message("User module list:")
foreach(i ${MODULE_NAME_LIST})
    message("\t${i}")
endforeach()

set(CLENCH_ENABLED_MODULES ${MODULE_NAME_LIST} CACHE STRING "Enabled module name list")
message("Enabled module list:")
foreach(i ${CLENCH_ENABLED_MODULES})
    message("\t${i}")
endforeach()
