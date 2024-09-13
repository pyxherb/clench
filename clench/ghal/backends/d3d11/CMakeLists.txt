find_package(FXC REQUIRED)
find_package(file2c REQUIRED)

file(GLOB HEADERS *.h)
file(GLOB SRC *.cc)

target_sources(clcghal_targets PUBLIC ${HEADERS} ${SRC})

target_compile_definitions(clcghal_targets PUBLIC CLCGHAL_HAS_D3D11_BACKEND=1)
target_link_libraries(clcghal_targets PUBLIC d3d11)

fxc_compile_shader(
    clcghal_d3d11_clear_rtv_vert_shader
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders/clear_rtv_vert.hlsl
    ${CMAKE_CURRENT_BINARY_DIR}/shaders/clear_rtv_vert.cso
    "vs_4_0"
    "VS"
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

fxc_compile_shader(
    clcghal_d3d11_clear_rtv_frag_shader
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders/clear_rtv_frag.hlsl
    ${CMAKE_CURRENT_BINARY_DIR}/shaders/clear_rtv_frag.cso
    "ps_4_0"
    "PS"
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

add_file2c_target(
    TARGET_NAME clcghal_d3d11_clear_rtv_vert_shader_cblob
    INPUT_FILE ${CMAKE_CURRENT_BINARY_DIR}/shaders/clear_rtv_vert.cso
    OUTPUT_FILE ${CMAKE_CURRENT_BINARY_DIR}/shaders/clear_rtv_vert.cc
    VAR_NAME clench::ghal::g_clearRtvVertShaderByteCode
    INCLUDE_FILES "<clench/ghal/backends/d3d11/backend.h>"
    PREFIXES "CLCGHAL_API"
)

add_file2c_target(
    TARGET_NAME clcghal_d3d11_clear_rtv_frag_shader_cblob
    INPUT_FILE ${CMAKE_CURRENT_BINARY_DIR}/shaders/clear_rtv_frag.cso
    OUTPUT_FILE ${CMAKE_CURRENT_BINARY_DIR}/shaders/clear_rtv_frag.cc
    VAR_NAME clench::ghal::g_clearRtvFragShaderByteCode
    INCLUDE_FILES "<clench/ghal/backends/d3d11/backend.h>"
    PREFIXES "CLCGHAL_API"
)

target_sources(clcghal_targets PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}/shaders/clear_rtv_vert.cc
    ${CMAKE_CURRENT_BINARY_DIR}/shaders/clear_rtv_frag.cc)
