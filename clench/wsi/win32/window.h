#ifndef _CLENCH_WSI_WIN32_WINDOW_H_
#define _CLENCH_WSI_WIN32_WINDOW_H_

#include <clench/wsi/window.h>
#include <Windows.h>
#include <map>

#define CLENCH_WNDCLASS_NAME "CLENCH"

namespace clench {
	namespace wsi {
		extern CLCWSI_API std::map<HWND, Window*> g_win32CreatedWindows;
	}
}

#endif
