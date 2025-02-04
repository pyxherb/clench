#include "except.h"

using namespace clench;
using namespace clench::ahal;

#pragma region ALSAPlatformSpecificException
CLCAHAL_API ALSAPlatformSpecificException::ALSAPlatformSpecificException(peff::Alloc *allocator, ALSAPlatformSpecificExceptionCode glPlatformSpecificExceptionCode) : PlatformSpecificException(allocator), glPlatformSpecificExceptionCode(glPlatformSpecificExceptionCode) {
}

CLCAHAL_API ALSAPlatformSpecificException::~ALSAPlatformSpecificException() {
}
#pragma endregion

#pragma region ErrorCreatingEGLSurfaceException
CLCAHAL_API ErrorCreatingEGLSurfaceException::ErrorCreatingEGLSurfaceException(peff::Alloc *allocator) : ALSAPlatformSpecificException(allocator, ALSAPlatformSpecificExceptionCode::ErrorCreatingEGLSurface) {
}

CLCAHAL_API ErrorCreatingEGLSurfaceException::~ErrorCreatingEGLSurfaceException() {
}

CLCAHAL_API const char *ErrorCreatingEGLSurfaceException::what() const {
	return "Error creating the EGL surface";
}

CLCAHAL_API void ErrorCreatingEGLSurfaceException::dealloc() {
	peff::destroyAndRelease<ErrorCreatingEGLSurfaceException>(allocator.get(), this, sizeof(std::max_align_t));
}

CLCAHAL_API ErrorCreatingEGLSurfaceException *ErrorCreatingEGLSurfaceException::alloc(peff::Alloc *allocator) {
	return peff::allocAndConstruct<ErrorCreatingEGLSurfaceException>(allocator, sizeof(std::max_align_t), allocator);
}
#pragma endregion

#pragma region ErrorInitializingGLLoaderException
CLCAHAL_API ErrorInitializingGLLoaderException::ErrorInitializingGLLoaderException(peff::Alloc *allocator) : ALSAPlatformSpecificException(allocator, ALSAPlatformSpecificExceptionCode::ErrorInitializingGLLoader) {
}

CLCAHAL_API ErrorInitializingGLLoaderException::~ErrorInitializingGLLoaderException() {
}

CLCAHAL_API const char *ErrorInitializingGLLoaderException::what() const {
	return "Error initializing the OpenGL loader";
}

CLCAHAL_API void ErrorInitializingGLLoaderException::dealloc() {
	peff::destroyAndRelease<ErrorInitializingGLLoaderException>(allocator.get(), this, sizeof(std::max_align_t));
}

CLCAHAL_API ErrorInitializingGLLoaderException *ErrorInitializingGLLoaderException::alloc(peff::Alloc *allocator) {
	return peff::allocAndConstruct<ErrorInitializingGLLoaderException>(allocator, sizeof(std::max_align_t), allocator);
}
#pragma endregion
