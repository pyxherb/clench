file(GLOB HEADERS *.h)
file(GLOB SRC *.cc)

target_sources(clcutils_targets PUBLIC ${HEADERS} ${SRC})
