file(GLOB HEADERS *.h)
file(GLOB SRC *.cc)

target_sources(clcacri PRIVATE ${HEADERS} ${SRC})
target_sources(clcacri_static PRIVATE ${HEADERS} ${SRC})
