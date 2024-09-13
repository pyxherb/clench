#ifndef _CLENCH_BASE_ATTRIBS_H_
#define _CLENCH_BASE_ATTRIBS_H_

#include <cassert>

#ifdef _WIN32

#define DLLEXPORT __declspec(dllexport)
#define DLLIMPORT __declspec(dllimport)
#define FORCEINLINE __forceinline
#define NOINLINE __declspec(noinline)

#elif defined(__unix__)

#define DLLEXPORT __attribute__((__visibility__("default")))
#define DLLIMPORT __attribute__((__visibility__("default")))
#define FORCEINLINE __attribute__((__always_inline__)) inline
#define NOINLINE __attribute__((__noinline__))

#else

#error Compiler is not supported

#endif

#endif
