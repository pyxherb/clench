file(GLOB HEADERS *.h)
file(GLOB SRC *.cc)

clench_add_component(clcutils)

target_sources(clcutils PRIVATE ${HEADERS} ${SRC})
target_sources(clcutils_static PRIVATE ${HEADERS} ${SRC})
target_compile_definitions(clcutils PRIVATE CLCUTILS_BUILDING=1)
target_compile_definitions(clcutils_static PRIVATE CLCUTILS_BUILDING=1)
target_link_libraries(clcutils PUBLIC clcbase)
target_link_libraries(clcutils_static PUBLIC clcbase_static)

if(WIN32)
    add_subdirectory("win32")
elseif(LINUX)
    add_subdirectory("linux")
endif()
