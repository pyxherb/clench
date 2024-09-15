#ifndef _CLENCH_MATH_BASEDEFS_H_
#define _CLENCH_MATH_BASEDEFS_H_

#include <clench/base/basedefs.h>
#include <clench/base/clsmacro.h>

#if CLENCH_DYNAMIC_LINK
	#if CLCMATH_BUILDING
		#define CLCMATH_API DLLEXPORT
	#else
		#define CLCMATH_API DLLIMPORT
	#endif
#else
	#define CLCMATH_API DLLEXPORT
#endif

#define CLCMATH_INDEX_ASSERT(index, maxIndex) CLENCH_ASSERT(index < maxIndex, "Out of range");

#endif
