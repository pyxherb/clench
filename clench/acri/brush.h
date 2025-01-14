#ifndef _CLENCH_ACRI_BRUSH_H_
#define _CLENCH_ACRI_BRUSH_H_

#include "resource.h"
#include <clench/ghal/color.h>
#include <cstdint>

namespace clench {
	namespace acri {
		enum class BrushType : uint8_t {
			SolidColor = 0,
			Gradient,
			Texture
		};

		class Brush : public ACRIResource {
		public:
			BrushType brushType;

			CLENCH_NO_COPY_MOVE_METHODS(Brush);

			CLCACRI_API Brush(ACRIContext *context, BrushType brushType);
			CLCACRI_API virtual ~Brush();
		};

		class SolidColorBrush : public Brush {
		public:
			ghal::Color color;

			CLENCH_NO_COPY_MOVE_METHODS(SolidColorBrush);

			CLCACRI_API SolidColorBrush(ACRIContext *context, const ghal::Color &color);
			CLCACRI_API virtual ~SolidColorBrush();

			CLCACRI_API virtual void dealloc() override;

			CLCACRI_API static SolidColorBrush *alloc(ACRIContext *context, peff::Alloc *allocator, const ghal::Color &color);
		};
	}
}

#endif
