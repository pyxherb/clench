#ifndef _CLENCH_BASE_ATTRIBS_H_
#define _CLENCH_BASE_ATTRIBS_H_

#include <cassert>

#if _MSC_VER

#define CLENCH_DLLEXPORT __declspec(dllexport)
#define CLENCH_DLLIMPORT __declspec(dllimport)
#define CLENCH_FORCEINLINE __forceinline
#define CLENCH_NOINLINE __declspec(noinline)

#elif defined(__GNUC__)

#define CLENCH_DLLEXPORT __attribute__((__visibility__("default")))
#define CLENCH_DLLIMPORT __attribute__((__visibility__("default")))
#define CLENCH_FORCEINLINE __attribute__((__always_inline__)) inline
#define CLENCH_NOINLINE __attribute__((__noinline__))

#else

#error The compiler is not supported

#endif

#endif
