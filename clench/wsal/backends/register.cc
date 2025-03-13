#include "../backend.h"

#if CLCWSAL_HAS_X11_BACKEND
	#include "x11/backend.h"
#endif

#if CLCWSAL_HAS_WIN32_BACKEND
	#include "win32/backend.h"
#endif

using namespace clench;
using namespace clench::wsal;

CLCWSAL_API bool clench::wsal::WSAL::addBuiltinBackends(peff::Alloc *allocator) {
#if CLCWSAL_HAS_X11_BACKEND
	Backend *x11Backend;
	if(!(x11Backend = X11Backend::alloc(allocator, allocator)))
		return false;
	if(!registerBackend(x11Backend))
		return false;
#endif

#if CLCWSAL_HAS_WIN32_BACKEND
	Backend *wsalBackend;
	if (!(wsalBackend = Win32Backend::alloc(allocator, allocator)))
		return false;
	if (!registerBackend(wsalBackend))
		return false;
#endif

	return true;
}
