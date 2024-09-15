file(GLOB HEADERS *.h)
file(GLOB SRC *.cc)

add_library(clcmodrt STATIC)
target_compile_definitions(clcmodrt PUBLIC CLENCH_IS_USER_MODULE=1)
clench_add_basic_definitions(clcmodrt)
set_target_properties(clcmodrt PROPERTIES CXX_STANDARD 17)

target_sources(clcmodrt PRIVATE ${HEADERS} ${SRC})

if(WIN32)
    add_subdirectory("win32")
elseif(UNIX)
    add_subdirectory("unix")
endif()
