file(GLOB HEADERS *.h)
file(GLOB SRC *.cc)

target_sources(clcutils PRIVATE ${HEADERS} ${SRC})
target_sources(clcutils_static PRIVATE ${HEADERS} ${SRC})
