include(FetchContent)

FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad
    GIT_TAG        v2.0.6
    GIT_SHALLOW    TRUE
    GIT_PROGRESS   TRUE
)

FetchContent_GetProperties(glad)
if(NOT glad_POPULATED)
    message("Fetching glad")
    FetchContent_MakeAvailable(glad)

    add_subdirectory("${glad_SOURCE_DIR}/cmake" glad_cmake)
    glad_add_library(glad SHARED REPRODUCIBLE EXCLUDE_FROM_ALL LOADER API gl:core=4.6 EXTENSIONS GL_ARB_bindless_texture GL_EXT_texture_compression_s3tc)
    glad_add_library(glad_static STATIC REPRODUCIBLE EXCLUDE_FROM_ALL LOADER API gl:core=4.6 EXTENSIONS GL_ARB_bindless_texture GL_EXT_texture_compression_s3tc)
endif()

file(GLOB HEADERS *.h)
file(GLOB SRC *.cc)

target_sources(clcghal PRIVATE ${HEADERS} ${SRC})

target_compile_definitions(clcghal PUBLIC CLCGHAL_HAS_OPENGL_BACKEND=1)
target_compile_definitions(clcghal_static PUBLIC CLCGHAL_HAS_OPENGL_BACKEND=1)
target_link_libraries(clcghal PUBLIC glad)
target_link_libraries(clcghal_static PUBLIC glad_static)

if(WIN32)
    target_link_libraries(clcghal PUBLIC opengl32)
    target_link_libraries(clcghal_static PUBLIC opengl32)
else()
    target_link_libraries(clcghal PUBLIC GL EGL)
    target_link_libraries(clcghal_static PUBLIC GL EGL)
endif()
