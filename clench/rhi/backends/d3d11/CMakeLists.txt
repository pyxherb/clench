file(GLOB HEADERS *.h)
file(GLOB SRC *.cc)

target_sources(clcrhi_targets PUBLIC ${HEADERS} ${SRC})

target_compile_definitions(clcrhi_targets PUBLIC CLCRHI_HAS_D3D11_BACKEND=1)
target_link_libraries(clcrhi_targets PUBLIC d3d11)
