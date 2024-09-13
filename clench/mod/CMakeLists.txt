file(GLOB HEADERS *.h)
file(GLOB SRC *.cc)

clench_add_component(clcmod)

target_sources(clcmod_targets PUBLIC ${HEADERS} ${SRC})
target_compile_definitions(clcmod_targets PRIVATE CLCMOD_BUILDING=1)
