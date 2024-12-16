file(GLOB HEADERS *.h)
file(GLOB SRC *.cc)

target_sources(clcutils PUBLIC ${HEADERS} ${SRC})
target_sources(clcutils_static PUBLIC ${HEADERS} ${SRC})
