file(GLOB HEADERS *.h)
file(GLOB SRC *.cc)

clench_add_component(clcrmf)

target_sources(clcrmf PRIVATE ${HEADERS} ${SRC})
target_sources(clcrmf_static PRIVATE ${HEADERS} ${SRC})
target_compile_definitions(clcrmf PRIVATE CLCRMF_BUILDING=1)
target_compile_definitions(clcrmf_static PRIVATE CLCRMF_BUILDING=1)
