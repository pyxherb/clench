file(GLOB HEADERS *.h)
file(GLOB SRC *.cc)

clench_add_component(clcbase)
target_compile_definitions(clcbase_targets PRIVATE CLCBASE_BUILDING=1)

target_sources(clcbase_targets PUBLIC ${HEADERS} ${SRC})
