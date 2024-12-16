file(GLOB SRC *.h *.cc)
add_executable(clcngine ${SRC})
set_target_properties(clcngine PROPERTIES CXX_STANDARD 17)
clench_add_basic_definitions(clcngine)

clench_add_component_dependency_to_target(clcngine clcutils)
clench_add_component_dependency_to_target(clcngine clcwsal)
clench_add_component_dependency_to_target(clcngine clcghal)
clench_add_component_dependency_to_target(clcngine clcvwc)
clench_add_component_dependency_to_target(clcngine clcplat)

get_target_property(_target_binary_dir clcngine BINARY_DIR)
