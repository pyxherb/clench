cmake_minimum_required(VERSION 3.13)
project(clench VERSION 0.1.0)

include_directories(${PROJECT_SOURCE_DIR})

include(${PROJECT_SOURCE_DIR}/cmake/ClenchBuild.cmake)

add_subdirectory("clench")
add_subdirectory("engine")
add_subdirectory("modules")

get_property(USER_MODULE_SO_PATH_LIST GLOBAL PROPERTY _CLENCH_USER_MODULE_SO_PATH_LIST)
get_property(USER_MODULE_NAME_LIST GLOBAL PROPERTY _CLENCH_USER_MODULE_NAME_LIST)

message("User module shared object list: ${USER_MODULE_SO_PATH_LIST}")
message("User module list: ${USER_MODULE_NAME_LIST}")
