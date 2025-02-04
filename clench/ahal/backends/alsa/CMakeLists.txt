find_package(ALSA REQUIRED)

file(GLOB HEADERS *.h)
file(GLOB SRC *.cc)

target_sources(clcahal PRIVATE ${HEADERS} ${SRC})
target_sources(clcahal_static PRIVATE ${HEADERS} ${SRC})

set_target_properties(clcahal PROPERTIES CLCAHAL_HAS_ALSA_BACKEND TRUE)
set_target_properties(clcahal_static PROPERTIES CLCAHAL_HAS_ALSA_BACKEND TRUE)
target_compile_definitions(clcahal PUBLIC CLCAHAL_HAS_ALSA_BACKEND=1)
target_compile_definitions(clcahal_static PUBLIC CLCAHAL_HAS_ALSA_BACKEND=1)
target_link_libraries(clcahal PUBLIC glad)
target_link_libraries(clcahal_static PUBLIC glad_static)

target_link_libraries(clcahal PUBLIC asound2)
target_link_libraries(clcahal_static PUBLIC asound2)
