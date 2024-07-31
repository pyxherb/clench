#ifndef _CLENCH_WSI_INIT_H_
#define _CLENCH_WSI_INIT_H_

#include "window.h"
#include <stdexcept>

namespace clench {
	namespace wsi {
		CLCWSI_API void init();
		CLCWSI_API void deinit();
	}
}

#endif
