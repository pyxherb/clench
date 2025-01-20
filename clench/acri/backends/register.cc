#include <clench/acri/backend.h>

#if CLCGHAL_HAS_OPENGL_BACKEND
	#include "opengl/backend.h"
#endif

using namespace clench;
using namespace clench::acri;

CLCACRI_API bool clench::acri::registerBuiltinBackends(peff::Alloc *selfAllocator) {
#if CLCGHAL_HAS_OPENGL_BACKEND
	if(!registerBackend(GLACRIBackend::alloc(selfAllocator))) {
		return false;
	}
#endif
}
