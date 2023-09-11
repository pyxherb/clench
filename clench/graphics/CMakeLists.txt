file(GLOB SRC *.h *.hpp *.hh *.c *.cpp *.cc)
target_sources(clench PUBLIC ${SRC})

set_property(TARGET clench PROPERTY CXX_STANDARD 17)

add_subdirectory("base")
add_subdirectory("backends")
add_subdirectory("codec")
