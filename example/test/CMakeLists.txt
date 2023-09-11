cmake_minimum_required (VERSION 3.13)

find_package(Eigen3 REQUIRED)

file(GLOB SRC *.h *.hpp *.hh *.c *.cpp *.cc)
add_executable(test ${SRC})

set_property(TARGET test PROPERTY CXX_STANDARD 17)

target_include_directories(clench PUBLIC ${EIGEN3_INCLUDE_DIRS})
target_link_libraries(test PRIVATE clench)
