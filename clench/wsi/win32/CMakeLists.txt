file(GLOB HEADERS *.h)
file(GLOB SRC *.cc)

target_sources(clcwsi_targets PUBLIC ${HEADERS} ${SRC})