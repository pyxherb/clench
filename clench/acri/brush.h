#ifndef _CLENCH_ACRI_BRUSH_H_
#define _CLENCH_ACRI_BRUSH_H_

#include "resource.h"
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
		};
	}
}

#endif
