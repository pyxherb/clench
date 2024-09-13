file(GLOB SRC *.h *.cc)
clench_add_module(dftboot MODULE_TARGET)
target_sources(${MODULE_TARGET} PUBLIC ${SRC})
target_link_libraries(${MODULE_TARGET} PRIVATE clcmodrt)
