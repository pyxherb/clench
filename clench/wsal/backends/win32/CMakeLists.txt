file(GLOB HEADERS *.h)
file(GLOB SRC *.cc)

target_sources(clcwsal PRIVATE ${HEADERS} ${SRC})
target_sources(clcwsal_static PRIVATE ${HEADERS} ${SRC})
target_compile_definitions(clcwsal PUBLIC CLCWSAL_HAS_WIN32_BACKEND=1)
target_compile_definitions(clcwsal_static PUBLIC CLCWSAL_HAS_WIN32_BACKEND=1)
