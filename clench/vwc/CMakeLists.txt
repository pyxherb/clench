clench_add_component(clcvwc)

file(GLOB HEADERS *.h)
file(GLOB SRC *.cc)

target_sources(clcvwc PRIVATE ${HEADERS} ${SRC})
target_sources(clcvwc_static PRIVATE ${HEADERS} ${SRC})
target_link_libraries(clcvwc PUBLIC clcwsal)
target_link_libraries(clcvwc_static PUBLIC clcwsal_static)
target_compile_definitions(clcvwc PRIVATE CLCVWC_BUILDING=1)
target_compile_definitions(clcvwc_static PRIVATE CLCVWC_BUILDING=1)
