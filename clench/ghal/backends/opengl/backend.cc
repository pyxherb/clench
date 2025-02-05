#include "backend.h"
#include "device.h"

using namespace clench;
using namespace clench::ghal;

#if _WIN32
CLCGHAL_API HMODULE clench::ghal::g_hOpenGL32Dll = NULL;
#elif __unix__
#endif
CLCGHAL_API GLBackend *clench::ghal::g_glBackend = nullptr;
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

CLCGHAL_API GLBackend::GLBackend(peff::Alloc *selfAllocator)
	: Backend("opengl", selfAllocator)
#if _WIN32
#else
	  ,
	  initializedEglDisplays(selfAllocator)
#endif
{
	g_glBackend = this;
}

CLCGHAL_API GLBackend::~GLBackend() {
}

CLCGHAL_API void GLBackend::dealloc() {
	peff::destroyAndRelease<GLBackend>(selfAllocator.get(), this, sizeof(std::max_align_t));
}

bool GLBackend::doInit() {
#if _WIN32
	if (!(g_hOpenGL32Dll = LoadLibraryA("opengl32.dll")))
		return false;
#endif
	return true;
}

bool GLBackend::doDeinit() {
#if _WIN32
	if (!FreeLibrary(g_hOpenGL32Dll))
		return false;
#endif
	return true;
}

CLCGHAL_API base::ExceptionPtr GLBackend::createDevice(Device *&ghalDeviceOut) {
	Device *ptr = GLDevice::alloc(peff::getDefaultAlloc(), peff::getDefaultAlloc(), this);
	if (!ptr)
		return base::OutOfMemoryException::alloc();
	ghalDeviceOut = ptr;
	return {};
}

CLCGHAL_API GLBackend *GLBackend::alloc(peff::Alloc *selfAllocator) {
	return peff::allocAndConstruct<GLBackend>(selfAllocator, sizeof(std::max_align_t), selfAllocator);
}
