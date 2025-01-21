#ifndef _CLENCH_WSAL_UNIX_WINDOW_H_
#define _CLENCH_WSAL_UNIX_WINDOW_H_

#include "window.h"
#include <initializer_list>

namespace clench {
	namespace wsal {
		extern std::initializer_list<std::pair<unsigned int, KeyboardKeyCode>> x11KeyMap;
	}
}

#endif
