file(GLOB HEADERS *.h)
file(GLOB SRC *.cc)

clench_add_component(clcmod)

target_sources(clcmod PRIVATE ${HEADERS} ${SRC})
target_compile_definitions(clcmod PRIVATE CLCMOD_BUILDING=1)
target_link_libraries(clcmod PUBLIC clcutils)
