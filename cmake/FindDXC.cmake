message(CHECK_START "Finding dxc")

find_program(DXC_EXECUTABLE dxc NAMES dxc)

if (DXC_EXECUTABLE)
	message(CHECK_PASS "Found dxc: ${DXC_EXECUTABLE}")
	
	function(dxc_compile_shader
		target_name
		source_path output_path
		shader_profile
		entrypoint_name
		include_paths)
		set(_dxc_full_command ${DXC_EXECUTABLE} -T ${shader_profile} -Fo ${output_path} -E ${entrypoint_name} ${source_path})
		foreach(i ${include_paths})
			set(_dxc_full_command ${_dxc_full_command} -I ${i})
		endforeach()
		add_custom_target(
			${target_name}
			COMMAND ${_dxc_full_command}
			BYPRODUCTS ${output_path}
			SOURCES ${source_path}
		)
	endfunction()
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
    DXC
    REQUIRED_VARS DXC_EXECUTABLE)
