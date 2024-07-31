clench_add_user_component(clcwsi)

file(GLOB HEADERS *.h)
file(GLOB SRC *.cc)

target_sources(clcwsi_targets PUBLIC ${HEADERS} ${SRC})

if(WIN32)
	add_subdirectory("win32")
elseif(unix)
	add_subdirectory("unix")
endif()
