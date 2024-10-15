file(GLOB HEADERS *.h)
file(GLOB SRC *.cc)

target_sources(clcghal PRIVATE ${HEADERS} ${SRC})

target_compile_definitions(clcghal PUBLIC CLCGHAL_HAS_D3D11_BACKEND=1)
target_link_libraries(clcghal PUBLIC d3d11)
