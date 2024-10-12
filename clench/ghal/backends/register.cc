#include "../backend.h"

#if CLCGHAL_HAS_OPENGL_BACKEND
	#include "opengl/backend.h"
#endif

#if CLCGHAL_HAS_D3D11_BACKEND
	#include "d3d11/backend.h"
#endif

using namespace clench;
using namespace clench::ghal;

CLCGHAL_API void clench::ghal::registerBuiltinGHALBackends() {
#if CLCGHAL_HAS_OPENGL_BACKEND
	registerGHALBackend(new GLGHALBackend());
#endif

#if CLCGHAL_HAS_D3D11_BACKEND
	registerGHALBackend(new D3D11GHALBackend());
#endif
}
