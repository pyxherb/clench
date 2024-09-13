file(GLOB HEADERS *.h)
file(GLOB SRC *.cc)

target_sources(clcghal_targets PUBLIC ${HEADERS} ${SRC})

target_compile_definitions(clcghal_targets PUBLIC CLCGHAL_HAS_OPENGL_BACKEND=1)
