#ifndef _CLENCH_WSAL_WIN32_WINDOW_H_
#define _CLENCH_WSAL_WIN32_WINDOW_H_

#include <clench/wsal/window.h>
#include <Windows.h>
#include <windowsx.h>
#include <map>

#define CLENCH_WNDCLASS_NAME "CLENCH"

namespace clench {
	namespace wsal {
		extern CLCWSAL_API std::map<HWND, NativeWindow*> g_win32CreatedWindows;
	}
}

#endif
