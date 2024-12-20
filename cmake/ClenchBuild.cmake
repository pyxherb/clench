include_guard(GLOBAL)

set(CLENCH_LINK_MODE "dyn" CACHE STRING "Indicates the build mode of Clench engine")

function(clench_copy_output_so target_name component_name)
	get_target_property(_component_binary_dir ${component_name} BINARY_DIR)
	get_target_property(_target_binary_dir ${target_name} BINARY_DIR)
	set(_so_name "${CMAKE_SHARED_LIBRARY_PREFIX}${component_name}${CMAKE_SHARED_LIBRARY_SUFFIX}")

	add_custom_target(
		${target_name}_${component_name}_copy_so ALL
		COMMAND ${CMAKE_COMMAND}
			-E copy_if_different
			${_component_binary_dir}/${_so_name}
			${_target_binary_dir}/${_so_name}
		BYPRODUCTS ${_target_binary_dir}/${_so_name}
		DEPENDS ${component_name})
	add_dependencies(${target_name} ${target_name}_${component_name}_copy_so)
endfunction()

function(clench_add_basic_definitions target_name)
	if(${CLENCH_LINK_MODE} STREQUAL "dyn")
		target_compile_definitions(${target_name} PRIVATE CLENCH_DYNAMIC_LINK=1)
	elseif(${CLENCH_LINK_MODE} STREQUAL "static")
		target_compile_definitions(${target_name} PRIVATE CLENCH_STATIC_LINK=1)
	endif()
endfunction()

function(clench_add_component component_name)
	add_library(${component_name} SHARED)
	add_library(${component_name}_static STATIC)

	target_compile_definitions(${component_name} PRIVATE CLENCH_DYNAMIC_LINK=1)
	target_compile_definitions(${component_name}_static PRIVATE CLENCH_STATIC_LINK=1)
	set_target_properties(${component_name} PROPERTIES CXX_STANDARD 17)
	set_target_properties(${component_name}_static PROPERTIES CXX_STANDARD 17)
endfunction()

function(clench_add_component_dependency_to_target target_name component_name)
	if(${CLENCH_LINK_MODE} STREQUAL "dyn")
		target_link_libraries(${target_name} PUBLIC ${component_name})

		clench_copy_output_so(${target_name} ${component_name})
	elseif(${CLENCH_LINK_MODE} STREQUAL "static")
		target_link_libraries(${target_name} PUBLIC ${component_name}_static)
	endif()
endfunction()
