file(GLOB HEADERS *.h)
file(GLOB SRC *.cc)

target_sources(clcplat PRIVATE ${HEADERS} ${SRC})
