include_guard(GLOBAL)

set(CLENCH_LINK_MODE "shared" CACHE STRING "Indicates the build mode of Clench engine")

define_property(GLOBAL PROPERTY CLENCH_COMPONENT_NAME_LIST)
define_property(GLOBAL PROPERTY _CLENCH_MODULE_NAME_LIST)
define_property(GLOBAL PROPERTY _CLENCH_MODULE_SO_PATH_LIST)

function(clench_add_basic_definitions target_name)
	if(${CLENCH_LINK_MODE} STREQUAL "shared")
		target_compile_definitions(${target_name} PUBLIC CLENCH_SHARED_LINK=1)
	elseif(${CLENCH_LINK_MODE} STREQUAL "static")
		target_compile_definitions(${target_name} PUBLIC CLENCH_STATIC_LINK=1)
	endif()
endfunction()

function(clench_add_component component_name)
	add_library(${component_name}_targets OBJECT)

	if(${CLENCH_LINK_MODE} STREQUAL "shared")
		add_library(${component_name}_shared SHARED $<TARGET_PROPERTY:${component_name}_targets,SOURCES>)
		target_link_libraries(${component_name}_shared PUBLIC $<TARGET_PROPERTY:${component_name}_targets,LINK_LIBRARIES>)
		target_compile_definitions(${component_name}_shared PUBLIC $<TARGET_PROPERTY:${component_name}_targets,COMPILE_DEFINITIONS>)
		set_target_properties(${component_name}_shared PROPERTIES CXX_STANDARD 17)

		clench_add_basic_definitions(${component_name}_shared)

		add_library(${component_name} ALIAS ${component_name}_shared)
	elseif(${CLENCH_LINK_MODE} STREQUAL "static")
		add_library(${component_name}_static STATIC $<TARGET_PROPERTY:${component_name}_targets,SOURCES>)
		target_link_libraries(${component_name}_static PUBLIC $<TARGET_PROPERTY:${component_name}_targets,LINK_LIBRARIES>)
		target_compile_definitions(${component_name}_static PUBLIC $<TARGET_PROPERTY:${component_name}_targets,COMPILE_DEFINITIONS>)
		set_target_properties(${component_name}_static PROPERTIES CXX_STANDARD 17)

		clench_add_basic_definitions(${component_name}_static)

		add_library(${component_name} ALIAS ${component_name}_static)
	endif()

    get_property(COMPONENT_NAME_LIST GLOBAL PROPERTY _CLENCH_MODULE_NAME_LIST)
    list(APPEND COMPONENT_NAME_LIST ${component_name})
    set_property(GLOBAL PROPERTY CLENCH_COMPONENT_NAME_LIST ${COMPONENT_NAME_LIST})
endfunction()

function(clench_add_module module_name target_name_out)
	set(library_name "clcm_${module_name}")

    if(${CLENCH_LINK_MODE} STREQUAL "shared")
        add_library(${library_name} SHARED)

        set_target_properties(${library_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

        get_property(USER_MODULE_SO_PATH_LIST GLOBAL PROPERTY _CLENCH_MODULE_SO_PATH_LIST)
        list(APPEND USER_MODULE_SO_PATH_LIST $<TARGET_FILE:${library_name}>)
        set_property(GLOBAL PROPERTY _CLENCH_MODULE_SO_PATH_LIST ${USER_MODULE_SO_PATH_LIST})
		
		clench_add_basic_definitions(${library_name})
    elseif(${CLENCH_LINK_MODE} STREQUAL "static")
        add_library(${library_name} STATIC)
		target_link_libraries(${library_name} PUBLIC clcmodrt)
		
		clench_add_basic_definitions(${library_name})
    endif()

    target_compile_definitions(${library_name} PRIVATE -DCLENCH_IS_USER_MODULE=1)

    get_property(USER_MODULE_NAME_LIST GLOBAL PROPERTY _CLENCH_MODULE_NAME_LIST)
    list(APPEND USER_MODULE_NAME_LIST ${module_name})
    set_property(GLOBAL PROPERTY _CLENCH_MODULE_NAME_LIST ${USER_MODULE_NAME_LIST})

    set(${target_name_out} ${library_name} PARENT_SCOPE)
endfunction()
