find_package(file2c REQUIRED)

file(GLOB HEADERS *.h)
file(GLOB SRC *.cc)

add_file2c_target(
    TARGET_NAME triangle_solidcolor_fill_vertex_330
    INPUT_FILE ${CMAKE_CURRENT_SOURCE_DIR}/shaders/triangle_solidcolor_fill_vertex_330.glsl
    OUTPUT_FILE ${CMAKE_CURRENT_BINARY_DIR}/triangle_solidcolor_fill_vertex_330.cc
    VAR_NAME g_triangle_solidcolor_fill_vertex_330
    PREFIXES clench::acri::
    INCLUDE_FILES "<clench/acri/backends/opengl/device.h>"
)
list(APPEND SRC ${CMAKE_CURRENT_BINARY_DIR}/triangle_solidcolor_fill_vertex_330.cc)

add_file2c_target(
    TARGET_NAME triangle_solidcolor_fill_fragment_330
    INPUT_FILE ${CMAKE_CURRENT_SOURCE_DIR}/shaders/triangle_solidcolor_fill_fragment_330.glsl
    OUTPUT_FILE ${CMAKE_CURRENT_BINARY_DIR}/triangle_solidcolor_fill_fragment_330.cc
    VAR_NAME g_triangle_solidcolor_fill_fragment_330
    PREFIXES clench::acri::
    INCLUDE_FILES "<clench/acri/backends/opengl/device.h>"
)
list(APPEND SRC ${CMAKE_CURRENT_BINARY_DIR}/triangle_solidcolor_fill_fragment_330.cc)

add_file2c_target(
    TARGET_NAME ellipse_solidcolor_fill_vertex_330
    INPUT_FILE ${CMAKE_CURRENT_SOURCE_DIR}/shaders/ellipse_solidcolor_fill_vertex_330.glsl
    OUTPUT_FILE ${CMAKE_CURRENT_BINARY_DIR}/ellipse_solidcolor_fill_vertex_330.cc
    VAR_NAME g_ellipse_solidcolor_fill_vertex_330
    PREFIXES clench::acri::
    INCLUDE_FILES "<clench/acri/backends/opengl/device.h>"
)
list(APPEND SRC ${CMAKE_CURRENT_BINARY_DIR}/ellipse_solidcolor_fill_vertex_330.cc)

add_file2c_target(
    TARGET_NAME ellipse_solidcolor_fill_fragment_330
    INPUT_FILE ${CMAKE_CURRENT_SOURCE_DIR}/shaders/ellipse_solidcolor_fill_fragment_330.glsl
    OUTPUT_FILE ${CMAKE_CURRENT_BINARY_DIR}/ellipse_solidcolor_fill_fragment_330.cc
    VAR_NAME g_ellipse_solidcolor_fill_fragment_330
    PREFIXES clench::acri::
    INCLUDE_FILES "<clench/acri/backends/opengl/device.h>"
)
list(APPEND SRC ${CMAKE_CURRENT_BINARY_DIR}/ellipse_solidcolor_fill_fragment_330.cc)

add_file2c_target(
    TARGET_NAME ellipse_solidcolor_draw_vertex_330
    INPUT_FILE ${CMAKE_CURRENT_SOURCE_DIR}/shaders/ellipse_solidcolor_draw_vertex_330.glsl
    OUTPUT_FILE ${CMAKE_CURRENT_BINARY_DIR}/ellipse_solidcolor_draw_vertex_330.cc
    VAR_NAME g_ellipse_solidcolor_draw_vertex_330
    PREFIXES clench::acri::
    INCLUDE_FILES "<clench/acri/backends/opengl/device.h>"
)
list(APPEND SRC ${CMAKE_CURRENT_BINARY_DIR}/ellipse_solidcolor_draw_vertex_330.cc)

add_file2c_target(
    TARGET_NAME ellipse_solidcolor_draw_fragment_330
    INPUT_FILE ${CMAKE_CURRENT_SOURCE_DIR}/shaders/ellipse_solidcolor_draw_fragment_330.glsl
    OUTPUT_FILE ${CMAKE_CURRENT_BINARY_DIR}/ellipse_solidcolor_draw_fragment_330.cc
    VAR_NAME g_ellipse_solidcolor_draw_fragment_330
    PREFIXES clench::acri::
    INCLUDE_FILES "<clench/acri/backends/opengl/device.h>"
)
list(APPEND SRC ${CMAKE_CURRENT_BINARY_DIR}/ellipse_solidcolor_draw_fragment_330.cc)

target_sources(clcacri PRIVATE ${HEADERS} ${SRC})
target_sources(clcacri_static PRIVATE ${HEADERS} ${SRC})
