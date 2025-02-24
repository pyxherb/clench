#ifndef _CLENCH_GHAL_COLOR_H_
#define _CLENCH_GHAL_COLOR_H_

#include <clench/math/vec.h>

namespace clench {
	namespace ghal {
		/// @brief Color structure that presents RGBA colors in normalized [0.0, 1.0]
		struct Color {
			union {
				math::Vec4f vec;
				struct {
					float r, g, b, a;
				};
			};

			constexpr Color(float r, float g, float b, float a): r(r), g(g), b(b), a(a) {}
			CLENCH_FORCEINLINE Color(const math::Vec4f &vec): vec(vec) {}
		};
	}
}

#endif
