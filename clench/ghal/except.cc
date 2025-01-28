#include "except.h"

using namespace clench;
using namespace clench::ghal;

#pragma region GHALException
CLCGHAL_API GHALException::GHALException(peff::Alloc *allocator, GHALExceptionCode ghalExceptionCode) : Exception(allocator, EXCEPTION_TYPE_GHAL), ghalExceptionCode(ghalExceptionCode) {
}

CLCGHAL_API GHALException::~GHALException() {
}
#pragma endregion

#pragma region ErrorCreatingDeviceContextException
CLCGHAL_API ErrorCreatingDeviceContextException::ErrorCreatingDeviceContextException(peff::Alloc *allocator, base::ExceptionPtr &&minorException) : GHALException(allocator, GHALExceptionCode::ErrorCreatingDeviceContext), minorException(std::move(minorException)) {
}

CLCGHAL_API ErrorCreatingDeviceContextException::~ErrorCreatingDeviceContextException() {
}

CLCGHAL_API const char *ErrorCreatingDeviceContextException::what() const {
	return "Error creating the GHAL device context";
}

CLCGHAL_API void ErrorCreatingDeviceContextException::dealloc() {
	peff::destroyAndRelease<ErrorCreatingDeviceContextException>(allocator.get(), this, sizeof(std::max_align_t));
}

CLCGHAL_API ErrorCreatingDeviceContextException *ErrorCreatingDeviceContextException::alloc(peff::Alloc *allocator, base::ExceptionPtr &&minorException) {
	return peff::allocAndConstruct<ErrorCreatingDeviceContextException>(allocator, sizeof(std::max_align_t), allocator, std::move(minorException));
}
#pragma endregion

#pragma region PlatformSpecificException
CLCGHAL_API PlatformSpecificException::PlatformSpecificException(peff::Alloc *allocator) : GHALException(allocator, GHALExceptionCode::PlatformSpecific) {
}

CLCGHAL_API PlatformSpecificException::~PlatformSpecificException() {
}
#pragma endregion
