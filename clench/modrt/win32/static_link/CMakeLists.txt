file(GLOB HEADERS *.h)
file(GLOB SRC *.cc)

target_sources(clcmodrt PUBLIC ${HEADERS} ${SRC})
