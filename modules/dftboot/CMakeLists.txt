file(GLOB SRC *.h *.cc)
file(GLOB SRC *.h *.cc)
clench_add_module(dftboot MODULE_TARGET)
target_sources(${MODULE_TARGET} PRIVATE ${SRC})
