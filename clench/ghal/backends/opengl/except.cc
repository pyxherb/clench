#include "except.h"

using namespace clench;
using namespace clench::ghal;

#pragma region GLPlatformSpecificException
CLCGHAL_API GLPlatformSpecificException::GLPlatformSpecificException(peff::Alloc *allocator, GLPlatformSpecificExceptionCode glPlatformSpecificExceptionCode) : PlatformSpecificException(allocator), glPlatformSpecificExceptionCode(glPlatformSpecificExceptionCode) {
}

CLCGHAL_API GLPlatformSpecificException::~GLPlatformSpecificException() {
}
#pragma endregion

#pragma region ErrorCreatingEGLSurfaceException
CLCGHAL_API ErrorCreatingEGLSurfaceException::ErrorCreatingEGLSurfaceException(peff::Alloc *allocator) : GLPlatformSpecificException(allocator, GLPlatformSpecificExceptionCode::ErrorCreatingEGLSurface) {
}

CLCGHAL_API ErrorCreatingEGLSurfaceException::~ErrorCreatingEGLSurfaceException() {
}

CLCGHAL_API const char *ErrorCreatingEGLSurfaceException::what() const {
	return "Error creating the EGL surface";
}

CLCGHAL_API void ErrorCreatingEGLSurfaceException::dealloc() {
	peff::destroyAndRelease<ErrorCreatingEGLSurfaceException>(allocator.get(), this, sizeof(std::max_align_t));
}

CLCGHAL_API ErrorCreatingEGLSurfaceException *ErrorCreatingEGLSurfaceException::alloc(peff::Alloc *allocator) {
	return peff::allocAndConstruct<ErrorCreatingEGLSurfaceException>(allocator, sizeof(std::max_align_t), allocator);
}
#pragma endregion

#pragma region ErrorInitializingGLLoaderException
CLCGHAL_API ErrorInitializingGLLoaderException::ErrorInitializingGLLoaderException(peff::Alloc *allocator) : GLPlatformSpecificException(allocator, GLPlatformSpecificExceptionCode::ErrorInitializingGLLoader) {
}

CLCGHAL_API ErrorInitializingGLLoaderException::~ErrorInitializingGLLoaderException() {
}

CLCGHAL_API const char *ErrorInitializingGLLoaderException::what() const {
	return "Error initializing the OpenGL loader";
}

CLCGHAL_API void ErrorInitializingGLLoaderException::dealloc() {
	peff::destroyAndRelease<ErrorInitializingGLLoaderException>(allocator.get(), this, sizeof(std::max_align_t));
}

CLCGHAL_API ErrorInitializingGLLoaderException *ErrorInitializingGLLoaderException::alloc(peff::Alloc *allocator) {
	return peff::allocAndConstruct<ErrorInitializingGLLoaderException>(allocator, sizeof(std::max_align_t), allocator);
}
#pragma endregion
