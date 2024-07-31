set(CLCUTILS_LINK_LIBRARIES "")

file(GLOB HEADERS *.h)
file(GLOB SRC *.cc)

clench_add_user_component(clcutils)

target_sources(clcutils_targets PUBLIC ${HEADERS} ${SRC})

if(WIN32)
    add_subdirectory("win32")
elseif(LINUX)
    add_subdirectory("linux")
endif()
