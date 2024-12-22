#ifndef _CLENCH_ACRI_GRADIENT_H_
#define _CLENCH_ACRI_GRADIENT_H_

#include "resource.h"

namespace clench {
	namespace acri {
		enum class GradientType : uint8_t {
			Linear = 0,
			Radial,
		};

		class Gradient : public ACRIResource {
		public:
			GradientType gradientType;

			CLCACRI_API Gradient(GradientType gradientType);
			CLCACRI_API virtual ~Gradient();
		};
	}
}

#endif
