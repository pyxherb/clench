file(GLOB HEADERS *.h)
file(GLOB SRC *.cc)

target_sources(clcwsal_targets PUBLIC ${HEADERS} ${SRC})
