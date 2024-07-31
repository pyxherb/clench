file(GLOB HEADERS *.h)
file(GLOB SRC *.cc *.inl)

add_library(clcmath_targets OBJECT)
set_target_properties(clcmath_targets PROPERTIES CXX_STANDARD 17)

target_sources(clcmath_targets PUBLIC ${HEADERS} ${SRC})
