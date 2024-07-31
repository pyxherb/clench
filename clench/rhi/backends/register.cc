#include "../backend.h"

#if CLCRHI_HAS_OPENGL_BACKEND
#endif

#if CLCRHI_HAS_D3D11_BACKEND
	#include "d3d11/backend.h"
#endif

using namespace clench;
using namespace clench::rhi;

CLCRHI_API void clench::rhi::registerBuiltinRHIBackends() {
#if CLCRHI_HAS_OPENGL_BACKEND
#endif

#if CLCRHI_HAS_D3D11_BACKEND
	registerRHIBackend(new D3D11RHIBackend());
#endif
}
