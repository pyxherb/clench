file(GLOB HEADERS *.h)
file(GLOB SRC *.cc)

clench_add_component(clcacri)

target_sources(clcacri PRIVATE ${HEADERS} ${SRC})
target_sources(clcacri_static PRIVATE ${HEADERS} ${SRC})
target_compile_definitions(clcacri PRIVATE CLCACRI_BUILDING=1)
target_compile_definitions(clcacri_static PRIVATE CLCACRI_BUILDING=1)
target_link_libraries(clcacri PUBLIC clcghal)
target_link_libraries(clcacri_static PUBLIC clcghal_static)

add_subdirectory("backends")
