#ifndef _CLENCH_ACRI_GRADIENT_H_
#define _CLENCH_ACRI_GRADIENT_H_

#include "resource.h"
#include <clench/ghal/color.h>
#include <peff/containers/map.h>

namespace clench {
	namespace acri {
		enum class GradientType : uint8_t {
			Linear = 0,
			Radial,
		};

		class Gradient : public ACRIResource {
		public:
			GradientType gradientType;

			CLCACRI_API Gradient(ACRIDevice *device, GradientType gradientType);
			CLCACRI_API virtual ~Gradient();
		};

		class LinearGradient : public Gradient {
		public:
			float angle;
			peff::Map<float, ghal::Color> colorStops;

			CLCACRI_API LinearGradient(ACRIDevice *device);
			CLCACRI_API virtual ~LinearGradient();
		};
	}
}

#endif
