#include <clench/acri/backend.h>

#if CLCGHAL_HAS_OPENGL_BACKEND
	#include "opengl/backend.h"
#endif

using namespace clench;
using namespace clench::acri;

CLCACRI_API bool clench::acri::registerBuiltinBackends(peff::Alloc *selfAllocator) {
#if CLCGHAL_HAS_OPENGL_BACKEND
	peff::RcObjectPtr<GLACRIBackend> glBackend = GLACRIBackend::alloc(selfAllocator);
	if(!registerBackend(glBackend.get())) {
		return false;
	}

	peff::ScopeGuard openglUnregistererGuard([&glBackend]() noexcept {
		unregisterBackend(glBackend->name);
	});
#endif

#if CLCGHAL_HAS_OPENGL_BACKEND
	openglUnregistererGuard.release();
#endif
}
