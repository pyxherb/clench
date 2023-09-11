cmake_minimum_required (VERSION 3.13)

find_package(Eigen3 REQUIRED)

file(GLOB SRC *.h *.hpp *.hh *.c *.cpp *.cc)
add_library(clench ${SRC})

target_include_directories(clench PUBLIC ${EIGEN3_INCLUDE_DIRS})

target_link_libraries(
    clench PUBLIC
)

add_subdirectory("debug")
add_subdirectory("math")
add_subdirectory("utils")
add_subdirectory("graphics")
add_subdirectory("audio")
add_subdirectory("input")
add_subdirectory("l10n")
add_subdirectory("package")
add_subdirectory("window")
