#ifndef _CLENCH_WSI_MOUSE_H_
#define _CLENCH_WSI_MOUSE_H_

#include <cstdint>

namespace clench {
	namespace wsi {
		enum class MouseButton : uint8_t {
			Left,
			Middle,
			Right
		};
	}
}

#endif
