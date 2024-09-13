file(GLOB SRC *.h *.cc)
add_executable(clcngine ${SRC})
set_target_properties(clcngine PROPERTIES CXX_STANDARD 17)
target_link_libraries(clcngine PUBLIC clcutils clcwsal clcghal clcvwc clcmod)

foreach(i ${CLENCH_ENABLED_MODULES})
    target_link_libraries(clcngine PUBLIC "clcm_${i}")
endforeach()
