file(GLOB HEADERS *.h)
file(GLOB SRC *.cc)

clench_add_user_component(clcrhi)

target_sources(clcrhi_targets PUBLIC ${HEADERS} ${SRC})

add_subdirectory("backends")
