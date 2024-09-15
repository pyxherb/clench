clench_add_component(clcvwc)

file(GLOB HEADERS *.h)
file(GLOB SRC *.cc)

target_sources(clcvwc PRIVATE ${HEADERS} ${SRC})
target_link_libraries(clcvwc PUBLIC clcwsal)
target_compile_definitions(clcvwc PRIVATE CLCVWC_BUILDING=1)
