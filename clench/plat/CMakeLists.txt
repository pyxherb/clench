file(GLOB HEADERS *.h)
file(GLOB SRC *.cc)

clench_add_component(clcplat)

target_sources(clcplat PRIVATE ${HEADERS} ${SRC})
target_sources(clcplat_static PRIVATE ${HEADERS} ${SRC})
target_compile_definitions(clcplat PRIVATE CLCPLAT_BUILDING=1)
target_compile_definitions(clcplat_static PRIVATE CLCPLAT_BUILDING=1)

if(WIN32)
	add_subdirectory("win32")
elseif(linux)
	add_subdirectory("linux")
endif()
