file(GLOB HEADERS *.h)
file(GLOB SRC *.cc)

clench_add_component(clcplat)

target_sources(clcplat_targets PUBLIC ${HEADERS} ${SRC})
target_compile_definitions(clcplat_targets PRIVATE CLCPLAT_BUILDING=1)

if(WIN32)
	add_subdirectory("win32")
elseif(linux)
	add_subdirectory("linux")
endif()
