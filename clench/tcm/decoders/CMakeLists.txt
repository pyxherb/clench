file(GLOB HEADERS *.h)
file(GLOB SRC *.cc)

target_sources(clctcm PRIVATE ${HEADERS} ${SRC})

add_subdirectory("png")
