#ifndef _CLENCH_ACRI_BRUSH_H_
#define _CLENCH_ACRI_BRUSH_H_

#include "basedefs.h"
#include <cstdint>

namespace clench {
	namespace acri {
		enum class BrushType : uint8_t {
			SolidColor = 0,
			Gradient,
			Texture
		};
	}
}

#endif
