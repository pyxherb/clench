file(GLOB HEADERS *.h)
file(GLOB SRC *.cc)

add_library(clcplat_targets OBJECT)
set_target_properties(clcplat_targets PROPERTIES CXX_STANDARD 17)

target_sources(clcplat_targets PUBLIC ${HEADERS} ${SRC})

if(WIN32)
	add_subdirectory("win32")
elseif(linux)
	add_subdirectory("linux")
endif()

add_library(clcplat_static STATIC $<TARGET_PROPERTY:clcplat_targets,SOURCES>)
target_link_libraries(clcplat_static PUBLIC $<TARGET_PROPERTY:clcplat_targets,LINK_LIBRARIES>)

add_library(clcplat_shared SHARED $<TARGET_PROPERTY:clcplat_targets,SOURCES>)
target_link_libraries(clcplat_shared PUBLIC $<TARGET_PROPERTY:clcplat_targets,LINK_LIBRARIES>)

if(${CLENCH_BUILD_MODE} STREQUAL "development")
    add_library(clcplat ALIAS clcplat_shared)
elseif(${CLENCH_BUILD_MODE} STREQUAL "release")
    add_library(clcplat ALIAS clcplat_static)
endif()
