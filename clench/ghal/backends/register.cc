#include "../backend.h"

#if CLCGHAL_HAS_OPENGL_BACKEND
	#include "opengl/backend.h"
#endif

#if CLCGHAL_HAS_D3D11_BACKEND
	#include "d3d11/backend.h"
#endif

using namespace clench;
using namespace clench::ghal;

CLCGHAL_API void clench::ghal::registerBuiltinBackends(peff::Alloc *selfAllocator) {
#if CLCGHAL_HAS_OPENGL_BACKEND
	registerBackend(GLBackend::alloc(selfAllocator));
#endif

#if CLCGHAL_HAS_D3D11_BACKEND
	registerBackend(new D3D11Backend());
#endif
}
