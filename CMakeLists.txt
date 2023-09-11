cmake_minimum_required (VERSION 3.13)

project("clench" VERSION 0.1.0)

#
# Check for AddressSanitizer if user has chosen to enable it for debugging.
#
if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(ENABLE_ADDRESS_SANITIZER TRUE CACHE BOOL "With AddressSanitizer")

    if(${ENABLE_ADDRESS_SANITIZER})
        message(CHECK_START "Finding AddressSanitizer")

        if(CMAKE_COMPILER_IS_GNUCXX OR (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang"))
            # GCC/Clang
            message(CHECK_PASS "AddressSanitizer mode: GCC/Clang")
            set(CMAKE_CXX_FLAGS "-fsanitize=address -fno-omit-frame-pointer -fsanitize-recover=address")
        elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
            message(CHECK_FAIL "You should enable AddressSanitizer manually in CMakeSettings.json")
        else()
            # Incompatible compiler
            message(CHECK_FAIL "No suitable AddressSanitizer mode for the compiler: \"${CMAKE_CXX_COMPILER_ID}\"")
        endif()
    endif()
endif()

add_compile_definitions(_CRT_SECURE_NO_WARNINGS)

set(CMAKE_CXX_STANDARD 17)
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/include)

set(CMAKE_PREFIX_PATH "${PROJECT_SOURCE_DIR}/cmake/")

add_subdirectory("clench")
add_subdirectory("example")
