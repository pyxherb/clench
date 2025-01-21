#include "../backend.h"

#if CLCWSAL_HAS_X11_BACKEND
	#include "x11/backend.h"
#endif

using namespace clench;
using namespace clench::wsal;

CLCWSAL_API bool clench::wsal::registerBuiltinWSALBackends(peff::Alloc *selfAllocator, peff::Alloc *resourceAllocator) {
#if CLCWSAL_HAS_X11_BACKEND
	registerWSALBackend(X11Backend::alloc(selfAllocator, resourceAllocator));
#endif

	return true;
}
