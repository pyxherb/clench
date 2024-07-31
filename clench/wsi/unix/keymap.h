#ifndef _CLENCH_WSI_UNIX_WINDOW_H_
#define _CLENCH_WSI_UNIX_WINDOW_H_

#include <clench/wsi/window.h>
#include <map>

namespace clench {
	namespace wsi {
		extern std::map<unsigned int, KeyboardKeyCode> x11KeyMap;
	}
}

#endif
