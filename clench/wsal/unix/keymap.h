#ifndef _CLENCH_WSAL_UNIX_WINDOW_H_
#define _CLENCH_WSAL_UNIX_WINDOW_H_

#include <clench/wsal/window.h>
#include <map>

namespace clench {
	namespace wsal {
		extern std::map<unsigned int, KeyboardKeyCode> x11KeyMap;
	}
}

#endif
