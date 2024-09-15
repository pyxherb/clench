file(GLOB SRC *.h *.cc)
clench_add_module(world3d MODULE_TARGET)
target_sources(${MODULE_TARGET} PRIVATE ${SRC})
