file(GLOB HEADERS *.h)
file(GLOB SRC *.cc)

clench_add_component(clcghal)

target_sources(clcghal PRIVATE ${HEADERS} ${SRC})
target_sources(clcghal_static PRIVATE ${HEADERS} ${SRC})
target_compile_definitions(clcghal PRIVATE CLCGHAL_BUILDING=1)
target_compile_definitions(clcghal_static PRIVATE CLCGHAL_BUILDING=1)
target_link_libraries(clcghal PUBLIC clcwsal clcutils)
target_link_libraries(clcghal_static PUBLIC clcwsal_static clcutils_static)

add_subdirectory("backends")
add_subdirectory("cpu")
