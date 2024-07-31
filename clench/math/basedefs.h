#ifndef _CLENCH_MATH_BASEDEFS_H_
#define _CLENCH_MATH_BASEDEFS_H_

#include <clench/base/basedefs.h>
#include <clench/base/clsmacro.h>

#define CLCMATH_API CLENCH_API

#define CLCMATH_INDEX_ASSERT(index, maxIndex) CLENCH_ASSERT(index < maxIndex, "Out of range");

#endif
