file(GLOB HEADERS *.h)
file(GLOB SRC *.cc)

target_sources(clcplat_targets PUBLIC ${HEADERS} ${SRC})