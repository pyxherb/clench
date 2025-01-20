file(GLOB HEADERS *.h)
file(GLOB SRC *.cc)

target_sources(clcacri PRIVATE ${HEADERS} ${SRC})
target_sources(clcacri_static PRIVATE ${HEADERS} ${SRC})

get_target_property(CLCGHAL_HAS_OPENGL_BACKEND clcghal CLCGHAL_HAS_OPENGL_BACKEND)

if(CLCGHAL_HAS_OPENGL_BACKEND)
    add_subdirectory("opengl")
endif()
