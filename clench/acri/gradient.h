#ifndef _CLENCH_ACRI_GRADIENT_H_
#define _CLENCH_ACRI_GRADIENT_H_

#include "basedefs.h"
#include <clench/utils/rcobj.h>
#include <cstdint>

namespace clench {
	namespace acri {
		enum class GradientType : uint8_t {
			Linear = 0,
			Radial,
		};

		class Gradient : public utils::RcObject {
		public:
			GradientType gradientType;

			CLCACRI_API Gradient(GradientType gradientType);
		};
	}
}

#endif
