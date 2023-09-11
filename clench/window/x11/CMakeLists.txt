file(GLOB SRC *.h *.hpp *.hh *.c *.cpp *.cc)
target_sources(clench PUBLIC ${SRC})
target_link_libraries(clench PUBLIC X11)
