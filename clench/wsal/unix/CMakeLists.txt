file(GLOB HEADERS *.h)
file(GLOB SRC *.cc)

target_sources(clcwsal PRIVATE ${HEADERS} ${SRC})
target_sources(clcwsal_static PRIVATE ${HEADERS} ${SRC})
target_link_libraries(clcwsal PUBLIC X11)
target_link_libraries(clcwsal_static PUBLIC X11)
