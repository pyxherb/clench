find_package(peff REQUIRED)

file(GLOB HEADERS *.h)
file(GLOB SRC *.cc)

clench_add_component(clcahal)

target_sources(clcahal PRIVATE ${HEADERS} ${SRC})
target_sources(clcahal_static PRIVATE ${HEADERS} ${SRC})
target_compile_definitions(clcahal PRIVATE CLCAHAL_BUILDING=1)
target_compile_definitions(clcahal_static PRIVATE CLCAHAL_BUILDING=1)
target_link_libraries(clcahal PUBLIC clcwsal clcutils peff_base peff_utils peff_containers)
target_link_libraries(clcahal_static PUBLIC clcwsal_static clcutils_static peff_base_static peff_utils_static peff_containers_static)

add_subdirectory("backends")
