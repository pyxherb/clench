file(GLOB HEADERS *.h)
file(GLOB SRC *.cc)

target_sources(clcahal PRIVATE ${HEADERS} ${SRC})
target_sources(clcahal_static PRIVATE ${HEADERS} ${SRC})

if(LINUX)
    add_subdirectory("alsa")
endif()

