file(GLOB HEADERS *.h)
file(GLOB SRC *.cc)

add_library(clcmodrt STATIC)
target_compile_definitions(clcmodrt PUBLIC CLENCH_IS_USER_MODULE=1)
clench_add_basic_definitions(clcmodrt)

target_sources(clcmodrt PUBLIC ${HEADERS} ${SRC})

if(WIN32)
    add_subdirectory("win32")
elseif(UNIX)
    add_subdirectory("unix")
endif()
