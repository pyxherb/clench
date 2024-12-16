file(GLOB HEADERS *.h)
file(GLOB SRC *.cc *.inl)

clench_add_component(clcmath)
target_compile_definitions(clcmath PRIVATE CLCMATH_BUILDING=1)
target_compile_definitions(clcmath_static PRIVATE CLCMATH_BUILDING=1)

target_sources(clcmath PRIVATE ${HEADERS} ${SRC})
target_sources(clcmath_static PRIVATE ${HEADERS} ${SRC})
