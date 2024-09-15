include_guard(GLOBAL)

set(CLENCH_LINK_MODE "dyn" CACHE STRING "Indicates the build mode of Clench engine")

define_property(GLOBAL PROPERTY CLENCH_COMPONENT_NAME_LIST)
define_property(GLOBAL PROPERTY _CLENCH_MODULE_NAME_LIST)
define_property(GLOBAL PROPERTY _CLENCH_MODULE_SO_PATH_LIST)

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
	if(${CLENCH_LINK_MODE} STREQUAL "dyn")
		add_library(${component_name} SHARED)
	elseif(${CLENCH_LINK_MODE} STREQUAL "static")
		add_library(${component_name} STATIC)
	else()
		message(FATAL_ERROR "Unrecognized linking mode '${CLENCH_LINK_MODE}'")
	endif()
	
	clench_add_basic_definitions(${component_name})
	set_target_properties(${component_name} PROPERTIES CXX_STANDARD 17)

    get_property(COMPONENT_NAME_LIST GLOBAL PROPERTY _CLENCH_MODULE_NAME_LIST)
    list(APPEND COMPONENT_NAME_LIST ${component_name})
    set_property(GLOBAL PROPERTY CLENCH_COMPONENT_NAME_LIST ${COMPONENT_NAME_LIST})
endfunction()

function(clench_add_component_dependency_to_target target_name component_name)
	if(${CLENCH_LINK_MODE} STREQUAL "dyn")
		target_link_libraries(${target_name} PUBLIC ${component_name})

		clench_copy_output_so(${target_name} ${component_name})
	elseif(${CLENCH_LINK_MODE} STREQUAL "static")
		target_link_libraries(${target_name} PUBLIC ${component_name})
	endif()
endfunction()

function(clench_add_module module_name target_name_out)
	set(library_name clcm_${module_name})

    if(${CLENCH_LINK_MODE} STREQUAL "dyn")
        add_library(${library_name} SHARED)
		target_link_libraries(${library_name} PUBLIC clcmod clcmodrt)

		clench_add_basic_definitions(${library_name})
    elseif(${CLENCH_LINK_MODE} STREQUAL "static")
		#
		# Note that we use object library to make sure that the modules will
		# always be linked with the executable.
		#
		# See: https://devblogs.microsoft.com/oldnewthing/20140321-00/?p=1433
		#
        add_library(${library_name} OBJECT)
		target_link_libraries(${library_name} PUBLIC clcmod clcmodrt)

		clench_add_basic_definitions(${library_name})
    endif()

    target_compile_definitions(${library_name} PRIVATE CLENCH_IS_USER_MODULE=1)
	set_target_properties(${library_name} PROPERTIES CXX_STANDARD 17)

    get_property(USER_MODULE_NAME_LIST GLOBAL PROPERTY _CLENCH_MODULE_NAME_LIST)
    list(APPEND USER_MODULE_NAME_LIST ${module_name})
    set_property(GLOBAL PROPERTY _CLENCH_MODULE_NAME_LIST ${USER_MODULE_NAME_LIST})

    set(${target_name_out} ${library_name} PARENT_SCOPE)
endfunction()

function(clench_add_module_dependency_to_target target_name module_name)
	set(library_name "clcm_${module_name}")
	if(${CLENCH_LINK_MODE} STREQUAL "dyn")
		target_link_libraries(${target_name} PRIVATE ${library_name})

		clench_copy_output_so(${target_name} ${library_name})
	elseif(${CLENCH_LINK_MODE} STREQUAL "static")
		target_link_libraries(${target_name} PRIVATE ${library_name})
	endif()
endfunction()
