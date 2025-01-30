file(GLOB SRC *.h *.cc)
add_executable(clcedit ${SRC})
set_target_properties(clcedit PROPERTIES CXX_STANDARD 17)
target_link_libraries(clcedit PUBLIC clcutils_static clcwsal_static clcghal_static clcacri_static clcvwc_static)
