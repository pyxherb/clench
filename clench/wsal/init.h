#ifndef _CLENCH_WSAL_INIT_H_
#define _CLENCH_WSAL_INIT_H_

#include "window.h"
#include <stdexcept>

namespace clench {
	namespace wsal {
		CLCWSAL_API void init();
		CLCWSAL_API void deinit();
	}
}

#endif
