set(CLCUTILS_LINK_LIBRARIES "")

file(GLOB HEADERS *.h)
file(GLOB SRC *.cc)

clench_add_component(clcutils)

target_sources(clcutils_targets PUBLIC ${HEADERS} ${SRC})
target_compile_definitions(clcutils_targets PRIVATE CLCUTILS_BUILDING=1)

if(WIN32)
    add_subdirectory("win32")
elseif(LINUX)
    add_subdirectory("linux")
endif()
