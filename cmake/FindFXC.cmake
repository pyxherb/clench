message(CHECK_START "Finding fxc")

find_program(FXC_EXECUTABLE fxc NAMES fxc)

if (FXC_EXECUTABLE)
	message(CHECK_PASS "Found fxc: ${FXC_EXECUTABLE}")
	
	function(fxc_compile_shader
		target_name
		source_path output_path
		shader_profile
		entrypoint_name
		include_paths)
		set(_fxc_full_command ${FXC_EXECUTABLE} /T ${shader_profile} /Fo ${output_path} /E ${entrypoint_name} ${source_path})
		foreach(i ${include_paths})
			set(_fxc_full_command ${_fxc_full_command} /I ${i})
		endforeach()
		add_custom_target(
			${target_name}
			COMMAND ${_fxc_full_command}
			BYPRODUCTS ${output_path}
			SOURCES ${source_path}
		)
	endfunction()
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
    FXC
    REQUIRED_VARS FXC_EXECUTABLE)
