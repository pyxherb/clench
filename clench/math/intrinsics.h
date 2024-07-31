#ifndef _CLENCH_MATH_INTRINSICS_H_
#define _CLENCH_MATH_INTRINSICS_H_

#include "basedefs.h"

#if (defined(_M_IX86) || defined(_M_X64) || __i386__ || __x86_64__)

	#define CLCMATH_X86_INTRINSICS 1

	#define CLCMATH_SSE_INTRINSICS 1

	#if defined(__AVX__)
		#define CLCMATH_AVX_INTRINSICS 1
	#endif

	#if defined(__AVX2__)
		#define CLCMATH_AVX2_INTRINSICS 1
	#endif

	#if defined(__F16C__)
		#define CLCMATH_F16C_INTRINSICS 1
	#endif

#endif

#if defined(CLCMATH_X86_INTRINSICS)
	#if defined(CLCMATH_SSE_INTRINSICS)
		#include <xmmintrin.h>
		#include <emmintrin.h>
	#endif
#endif

#endif
