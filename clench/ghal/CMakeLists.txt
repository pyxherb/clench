file(GLOB HEADERS *.h)
file(GLOB SRC *.cc)

clench_add_component(clcghal)

target_sources(clcghal PRIVATE ${HEADERS} ${SRC})
target_compile_definitions(clcghal PRIVATE CLCGHAL_BUILDING=1)
target_link_libraries(clcghal PUBLIC clcwsal clcutils)

add_subdirectory("backends")
add_subdirectory("cpu")
