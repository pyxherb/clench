find_package(ZLIB)
find_package(PNG)

if(ZLIB_FOUND AND PNG_FOUND)
    message("PNG codec enabled")

    file(GLOB SRC *.h *.hpp *.hh *.c *.cpp *.cc)
    target_sources(clench PUBLIC ${SRC})

    target_include_directories(clench PUBLIC ${ZLIB_INCLUDE_DIRS} ${PNG_INCLUDE_DIRS})
    target_link_libraries(clench PUBLIC ${ZLIB_LIBRARIES} ${PNG_LIBRARIES})

    target_compile_definitions(clench PUBLIC CLENCH_HAVE_IMGCODEC_PNG)
endif()
