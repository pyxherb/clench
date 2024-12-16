file(GLOB HEADERS *.h)
file(GLOB SRC *.cc)

target_sources(clcplat PRIVATE ${HEADERS} ${SRC})
target_sources(clcplat_static PRIVATE ${HEADERS} ${SRC})
target_link_libraries(clcplat PRIVATE iphlpapi)
target_link_libraries(clcplat_static PRIVATE iphlpapi)
