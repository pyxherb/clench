file(GLOB HEADERS *.h)
file(GLOB SRC *.cc)

target_sources(clcwsal PRIVATE ${HEADERS} ${SRC})
target_sources(clcwsal_static PRIVATE ${HEADERS} ${SRC})

if(WIN32)
	add_subdirectory("win32")
endif()

if(UNIX)
	add_subdirectory("x11")
endif()
