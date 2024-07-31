#ifndef _CLENCH_WSI_WIN32_KEYMAP_H_
#define _CLENCH_WSI_WIN32_KEYMAP_H_

#include <clench/wsi/window.h>
#include <Windows.h>

namespace clench {
	namespace wsi {
		extern CLCWSI_API const std::map<UINT, KeyboardKeyCode> g_win32KeyMap;
	}
}

#endif
