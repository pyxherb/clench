file(GLOB HEADERS *.h)
file(GLOB SRC *.cc)

target_sources(clcwsal PRIVATE ${HEADERS} ${SRC})
