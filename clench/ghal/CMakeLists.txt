file(GLOB HEADERS *.h)
file(GLOB SRC *.cc)

clench_add_component(clcghal)

target_sources(clcghal_targets PUBLIC ${HEADERS} ${SRC})
target_compile_definitions(clcghal_targets PRIVATE CLCGHAL_BUILDING=1)
target_link_libraries(clcghal_targets PUBLIC clcwsal clcutils)

add_subdirectory("backends")
add_subdirectory("cpu")
