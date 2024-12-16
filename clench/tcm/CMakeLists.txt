file(GLOB HEADERS *.h)
file(GLOB SRC *.cc)

clench_add_component(clctcm)

target_sources(clctcm PRIVATE ${HEADERS} ${SRC})
target_sources(clctcm_static PRIVATE ${HEADERS} ${SRC})
target_compile_definitions(clctcm PRIVATE CLCTCM_BUILDING=1)
target_compile_definitions(clctcm_static PRIVATE CLCTCM_BUILDING=1)

add_subdirectory("decoders")
