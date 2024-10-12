#include "backend.h"
#include "device.h"

using namespace clench;
using namespace clench::ghal;

#if _WIN32
CLCGHAL_API HMODULE clench::ghal::g_hOpenGL32Dll = NULL;
#elif __unix__
CLCGHAL_API std::map<EGLDisplay, size_t> clench::ghal::g_initializedEglDisplays;
#endif
CLCGHAL_API bool clench::ghal::g_glInitialized = false;

CLCGHAL_API void *clench::ghal::_loadGlProc(const char *name) {
#ifdef _WIN32
	void *addr = wglGetProcAddress(name);

	if (addr)
		return addr;

	if (!g_hOpenGL32Dll) {
		if (!(g_hOpenGL32Dll = LoadLibraryA("opengl32.dll")))
			return nullptr;
	}
	return GetProcAddress(g_hOpenGL32Dll, name);
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
