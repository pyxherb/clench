file(GLOB SRC *.h *.cc)
add_executable(clcngine ${SRC})
set_target_properties(clcngine PROPERTIES CXX_STANDARD 17)
target_link_libraries(clcngine PUBLIC clcutils clcwsi clcrhi)
