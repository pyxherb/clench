file(GLOB HEADERS *.h)
file(GLOB SRC *.cc)

target_sources(clcrhi_targets PUBLIC ${HEADERS} ${SRC})

target_compile_definitions(clcrhi_targets PUBLIC CLCRHI_HAS_OPENGL_BACKEND=1)