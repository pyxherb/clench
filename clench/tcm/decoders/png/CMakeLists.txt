find_package(PNG)

if(PNG_FOUND)
    file(GLOB HEADERS *.h)
    file(GLOB SRC *.cc)

    message("PNG TCM decoder enabled")

    target_sources(clctcm PRIVATE ${HEADERS} ${SRC})
    target_sources(clctcm_static PRIVATE ${HEADERS} ${SRC})
    target_compile_definitions(clctcm PUBLIC CLCTCM_HAS_PNG_DECODER=1)
    target_compile_definitions(clctcm_static PUBLIC CLCTCM_HAS_PNG_DECODER=1)

    target_link_libraries(clctcm PRIVATE ZLIB::ZLIB PNG::PNG)
    target_link_libraries(clctcm_static PRIVATE ZLIB::ZLIB PNG::PNG)
endif()
