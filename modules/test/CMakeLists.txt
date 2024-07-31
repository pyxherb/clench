file(GLOB SRC *.h *.cc)
clench_add_user_module(test MODULE_TARGET)
target_sources(${MODULE_TARGET} PRIVATE ${SRC})
target_link_libraries(${MODULE_TARGET} )