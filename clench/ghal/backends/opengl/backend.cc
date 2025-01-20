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

	return GetProcAddress(g_hOpenGL32Dll, name);
#else
	return (void *)eglGetProcAddress(name);
#endif
}

CLCGHAL_API GLGHALBackend::GLGHALBackend(peff::Alloc *selfAllocator) : GHALBackend("opengl", selfAllocator) {
#if _WIN32
	if (!(g_hOpenGL32Dll = LoadLibraryA("opengl32.dll")))
		throw std::runtime_error("Error loading OpenGL32.dll");
#endif
}

CLCGHAL_API GLGHALBackend::~GLGHALBackend() {
}

CLCGHAL_API GHALDevice *GLGHALBackend::createDevice() {
	return GLGHALDevice::alloc(peff::getDefaultAlloc(), peff::getDefaultAlloc(), this);
}

CLCGHAL_API GLGHALBackend *GLGHALBackend::alloc(peff::Alloc *selfAllocator) {
	return peff::allocAndConstruct<GLGHALBackend>(selfAllocator, sizeof(std::max_align_t), selfAllocator);
}
