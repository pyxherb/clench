clench_add_component(clcvwc)

file(GLOB HEADERS *.h)
file(GLOB SRC *.cc)

target_sources(clcvwc_targets PUBLIC ${HEADERS} ${SRC})
target_link_libraries(clcvwc_targets PUBLIC clcwsal)
target_compile_definitions(clcvwc_targets PRIVATE CLCVWC_BUILDING=1)
