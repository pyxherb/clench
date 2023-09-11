file(GLOB_RECURSE SRC *.h *.hpp *.hh *.c *.cpp *.cc)
target_sources(clench PUBLIC ${SRC})

if(WIN32)
    target_link_libraries(clench PUBLIC opengl32)
else()
    target_link_libraries(clench PUBLIC GL EGL)
endif()
