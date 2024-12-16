file(GLOB HEADERS *.h)
file(GLOB SRC *.cc)

target_sources(clctcm PRIVATE ${HEADERS} ${SRC})
target_sources(clctcm_static PRIVATE ${HEADERS} ${SRC})

add_subdirectory("png")
