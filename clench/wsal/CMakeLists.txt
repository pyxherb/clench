find_package(peff REQUIRED)

clench_add_component(clcwsal)

file(GLOB HEADERS *.h)
file(GLOB SRC *.cc)

target_sources(clcwsal PRIVATE ${HEADERS} ${SRC})
target_sources(clcwsal_static PRIVATE ${HEADERS} ${SRC})
target_compile_definitions(clcwsal PRIVATE CLCWSAL_BUILDING=1)
target_compile_definitions(clcwsal_static PRIVATE CLCWSAL_BUILDING=1)
target_link_libraries(clcwsal PUBLIC clcutils peff_base peff_containers)
target_link_libraries(clcwsal_static PUBLIC clcutils_static peff_base_static peff_containers_static)

add_subdirectory("backends")
