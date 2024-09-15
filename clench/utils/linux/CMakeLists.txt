file(GLOB HEADERS *.h)
file(GLOB SRC *.cc)

target_sources(clcutils PUBLIC ${HEADERS} ${SRC})
