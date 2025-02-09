#ifndef _CLENCH_MATH_MISC_H_
#define _CLENCH_MATH_MISC_H_

#include "basedefs.h"
#include <cstdint>

namespace clench {
	namespace math {
		CLCMATH_API float fmodf(float n, float d);
		CLCMATH_API double fmod(double n, double d);
	}
}

#endif
