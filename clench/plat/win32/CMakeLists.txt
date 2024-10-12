file(GLOB HEADERS *.h)
file(GLOB SRC *.cc)

target_sources(clcplat PRIVATE ${HEADERS} ${SRC})
target_link_libraries(clcplat PRIVATE iphlpapi)
