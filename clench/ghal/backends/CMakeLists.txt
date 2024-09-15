file(GLOB HEADERS *.h)
file(GLOB SRC *.cc)

target_sources(clcghal PRIVATE ${HEADERS} ${SRC})

add_subdirectory("opengl")

if(WIN32)
    add_subdirectory("d3d11")
endif()
