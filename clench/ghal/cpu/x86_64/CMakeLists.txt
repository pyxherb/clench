include(CMakeDetermineCompilerABI)

file(GLOB HEADERS_MSABI *_msabi.h *_msabi.inc)
file(GLOB SRC_MSABI *_msabi.asm)

file(GLOB HEADERS_SYSV *_sysv.h *_sysv.inc)
file(GLOB SRC_SYSV *_sysv.asm)

if(WIN32)
    set(HEADER ${HEADERS_MSABI})
    set(SRC ${SRC_MSABI})
else()
    set(HEADER ${HEADERS_SYSV})
    set(SRC ${SRC_SYSV})
endif()

target_sources(clcghal_targets PUBLIC ${HEADERS} ${SRC})
target_compile_definitions(clcghal_targets PRIVATE CLCGHAL_TEXCONV_OPTIMIZED_CODE=1)
