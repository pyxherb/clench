message(CHECK_START "Finding CPU optimized codes for GHAL")

set(CLENCH_GHAL_ENABLE_CPU_OPTIMIZED_CODES TRUE CACHE STRING "Enable CPU optimized codes for GHAL")

if(${CLENCH_GHAL_ENABLE_CPU_OPTIMIZED_CODES})
    if((${CMAKE_SYSTEM_PROCESSOR} STREQUAL "AMD64") OR
        (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64"))
        message(CHECK_PASS "x86_64")

        add_subdirectory("x86_64")
    else()
        message(CHECK_FAIL "not found")
    endif()
else()
    message(CHECK_FAIL "disabled")
endif()
