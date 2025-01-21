#ifndef _CLENCH_WSAL_WIN32_KEYMAP_H_
#define _CLENCH_WSAL_WIN32_KEYMAP_H_

#include <clench/wsal/window.h>
#include <Windows.h>

namespace clench {
	namespace wsal {
		extern CLCWSAL_API const std::initializer_list<std::pair<UINT, KeyboardKeyCode>> g_win32KeyMap;
	}
}

#endif
