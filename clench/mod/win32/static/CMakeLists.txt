file(GLOB HEADERS *.h)
file(GLOB SRC *.cc)

target_sources(clcmodrt PRIVATE ${HEADERS} ${SRC})
