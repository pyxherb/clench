file(GLOB HEADERS *.h)
file(GLOB SRC *.cc)

clench_add_component(clcbase)
target_compile_definitions(clcbase PRIVATE CLCBASE_BUILDING=1)

target_sources(clcbase PRIVATE ${HEADERS} ${SRC})
