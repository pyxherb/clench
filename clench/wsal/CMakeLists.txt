clench_add_component(clcwsal)

file(GLOB HEADERS *.h)
file(GLOB SRC *.cc)

target_sources(clcwsal_targets PUBLIC ${HEADERS} ${SRC})
target_compile_definitions(clcwsal_targets PRIVATE CLCWSAL_BUILDING=1)
target_link_libraries(clcwsal_targets PUBLIC clcutils)

if(WIN32)
	add_subdirectory("win32")
elseif(unix)
	add_subdirectory("unix")
endif()
