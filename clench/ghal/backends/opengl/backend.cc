#include "backend.h"
#include "device.h"

using namespace clench;
using namespace clench::ghal;

CLCGHAL_API std::map<EGLDisplay, size_t> clench::ghal::g_initializedEglDisplays;
CLCGHAL_API bool clench::ghal::g_glInitialized = false;

CLCGHAL_API void *clench::ghal::_loadGlProc(const char *name) {
#ifdef _WIN32
	void *addr = wglGetProcAddress(name);

	if (addr)
		return addr;

	return GetProcAddress(hOpenGL32Dll, name);
#else
	return (void *)eglGetProcAddress(name);
#endif
}

CLCGHAL_API GLGHALBackend::GLGHALBackend() : GHALBackend("opengl") {
}

CLCGHAL_API GLGHALBackend::~GLGHALBackend() {
}

CLCGHAL_API GHALDevice *GLGHALBackend::createDevice() {
	return new GLGHALDevice(this);
}
