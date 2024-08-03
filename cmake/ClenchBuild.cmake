include_guard(GLOBAL)

set(CLENCH_BUILD_MODE "development" CACHE STRING "Indicates the build mode of Clench engine")

define_property(GLOBAL PROPERTY CLENCH_COMPONENT_NAME_LIST)
define_property(GLOBAL PROPERTY _CLENCH_USER_MODULE_NAME_LIST)
define_property(GLOBAL PROPERTY _CLENCH_USER_MODULE_SO_PATH_LIST)

function(clench_add_user_component component_name)
	add_library(${component_name}_targets OBJECT)

	add_library(${component_name}_static STATIC $<TARGET_PROPERTY:${component_name}_targets,SOURCES>)
	target_link_libraries(${component_name}_static PUBLIC $<TARGET_PROPERTY:${component_name}_targets,LINK_LIBRARIES>)
	target_compile_definitions(${component_name}_static PUBLIC $<TARGET_PROPERTY:${component_name}_targets,COMPILE_DEFINITIONS>)
	set_target_properties(${component_name}_static PROPERTIES CXX_STANDARD 17)

	add_library(${component_name}_shared STATIC $<TARGET_PROPERTY:${component_name}_targets,SOURCES>)
	target_link_libraries(${component_name}_shared PUBLIC $<TARGET_PROPERTY:${component_name}_targets,LINK_LIBRARIES>)
	target_compile_definitions(${component_name}_shared PUBLIC $<TARGET_PROPERTY:${component_name}_targets,COMPILE_DEFINITIONS>)
	set_target_properties(${component_name}_shared PROPERTIES CXX_STANDARD 17)

	if(${CLENCH_BUILD_MODE} STREQUAL "development")
		add_library(${component_name} ALIAS ${component_name}_shared)
	elseif(${CLENCH_BUILD_MODE} STREQUAL "release")
		add_library(${component_name} ALIAS ${component_name}_static)
	endif()
	
    get_property(COMPONENT_NAME_LIST GLOBAL PROPERTY _CLENCH_USER_MODULE_NAME_LIST)
    list(APPEND COMPONENT_NAME_LIST ${component_name})
    set_property(GLOBAL PROPERTY CLENCH_COMPONENT_NAME_LIST ${COMPONENT_NAME_LIST})
endfunction()

function(clench_add_user_module module_name target_name_out)
    set(library_name "clcum_${module_name}")

    if(${CLENCH_BUILD_MODE} STREQUAL "development")
        add_library(${library_name} SHARED)

        set_target_properties(${library_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
        
        get_property(USER_MODULE_SO_PATH_LIST GLOBAL PROPERTY _CLENCH_USER_MODULE_SO_PATH_LIST)
        list(APPEND USER_MODULE_SO_PATH_LIST $<TARGET_FILE:${library_name}>)
        set_property(GLOBAL PROPERTY _CLENCH_USER_MODULE_SO_PATH_LIST ${USER_MODULE_SO_PATH_LIST})
    elseif(${CLENCH_BUILD_MODE} STREQUAL "release")
        add_library(${library_name} STATIC)
    endif()

    target_compile_definitions(${library_name} PRIVATE -DCLENCH_IS_USER_MODULE=1)

    get_property(USER_MODULE_NAME_LIST GLOBAL PROPERTY _CLENCH_USER_MODULE_NAME_LIST)
    list(APPEND USER_MODULE_NAME_LIST ${library_name})
    set_property(GLOBAL PROPERTY _CLENCH_USER_MODULE_NAME_LIST ${USER_MODULE_NAME_LIST})

    set(${target_name_out} ${library_name} PARENT_SCOPE)
endfunction()
