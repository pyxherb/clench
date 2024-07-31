file(GLOB HEADERS *.h)
file(GLOB SRC *.cc)

clench_add_user_component(clcbase)

target_sources(clcbase_targets PUBLIC ${HEADERS} ${SRC})
