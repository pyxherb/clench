clench_add_component(clcwsal)

file(GLOB HEADERS *.h)
file(GLOB SRC *.cc)

target_sources(clcwsal PRIVATE ${HEADERS} ${SRC})
target_sources(clcwsal_static PRIVATE ${HEADERS} ${SRC})
target_compile_definitions(clcwsal PRIVATE CLCWSAL_BUILDING=1)
target_compile_definitions(clcwsal_static PRIVATE CLCWSAL_BUILDING=1)
target_link_libraries(clcwsal PUBLIC clcutils)
target_link_libraries(clcwsal_static PUBLIC clcutils)

if(WIN32)
	add_subdirectory("win32")
elseif(UNIX)
	add_subdirectory("unix")
endif()
