file(GLOB HEADERS *.h)
file(GLOB SRC *.cc)

target_sources(clcghal PRIVATE ${HEADERS} ${SRC})

target_compile_definitions(clcghal PUBLIC CLCGHAL_HAS_OPENGL_BACKEND=1)
