find_package(peff REQUIRED)

file(GLOB HEADERS *.h)
file(GLOB SRC *.cc)

clench_add_component(clcbase)
target_compile_definitions(clcbase PRIVATE CLCBASE_BUILDING=1)
target_compile_definitions(clcbase_static PRIVATE CLCBASE_BUILDING=1)

target_sources(clcbase PRIVATE ${HEADERS} ${SRC})
target_sources(clcbase_static PRIVATE ${HEADERS} ${SRC})

target_link_libraries(clcbase PUBLIC peff_base)
target_link_libraries(clcbase_static PUBLIC peff_base_static)
