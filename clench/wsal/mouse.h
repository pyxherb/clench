#ifndef _CLENCH_WSAL_MOUSE_H_
#define _CLENCH_WSAL_MOUSE_H_

#include "basedefs.h"

namespace clench {
	namespace wsal {
		enum class MouseButton : uint8_t {
			Left,
			Middle,
			Right
		};

		class Window;

		CLCWSAL_API void setMouseCapture(Window *window);
		CLCWSAL_API void releaseMouseCapture();
		CLCWSAL_API Window *getMouseCapture();
	}
}

#endif
