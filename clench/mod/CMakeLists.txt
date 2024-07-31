file(GLOB HEADERS *.h)
file(GLOB SRC *.cc)

add_library(clcmod_targets OBJECT)
set_target_properties(clcmod_targets PROPERTIES CXX_STANDARD 17)

target_sources(clcmod_targets PUBLIC ${HEADERS} ${SRC})