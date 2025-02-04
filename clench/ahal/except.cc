#include "except.h"

using namespace clench;
using namespace clench::ahal;

#pragma region AHALException
CLCAHAL_API AHALException::AHALException(peff::Alloc *allocator, AHALExceptionCode ahalExceptionCode) : Exception(allocator, EXCEPTION_TYPE_AHAL), ahalExceptionCode(ahalExceptionCode) {
}

CLCAHAL_API AHALException::~AHALException() {
}
#pragma endregion

#pragma region ErrorOpeningDeviceException
CLCAHAL_API ErrorOpeningDeviceException::ErrorOpeningDeviceException(peff::Alloc *allocator) : AHALException(allocator, AHALExceptionCode::ErrorOpeningDevice) {
}

CLCAHAL_API ErrorOpeningDeviceException::~ErrorOpeningDeviceException() {
}

CLCAHAL_API const char *ErrorOpeningDeviceException::what() const {
	return "Error opening the AHAL device";
}

CLCAHAL_API void ErrorOpeningDeviceException::dealloc() {
	peff::destroyAndRelease<ErrorOpeningDeviceException>(allocator.get(), this, sizeof(std::max_align_t));
}

CLCAHAL_API ErrorOpeningDeviceException *ErrorOpeningDeviceException::alloc(peff::Alloc *allocator) {
	return peff::allocAndConstruct<ErrorOpeningDeviceException>(allocator, sizeof(std::max_align_t), allocator);
}
#pragma endregion

#pragma region ErrorCreatingDeviceContextException
CLCAHAL_API ErrorCreatingDeviceContextException::ErrorCreatingDeviceContextException(peff::Alloc *allocator, base::ExceptionPtr &&minorException) : AHALException(allocator, AHALExceptionCode::ErrorCreatingDeviceContext), minorException(std::move(minorException)) {
}

CLCAHAL_API ErrorCreatingDeviceContextException::~ErrorCreatingDeviceContextException() {
}

CLCAHAL_API const char *ErrorCreatingDeviceContextException::what() const {
	return "Error creating the AHAL device context";
}

CLCAHAL_API void ErrorCreatingDeviceContextException::dealloc() {
	peff::destroyAndRelease<ErrorCreatingDeviceContextException>(allocator.get(), this, sizeof(std::max_align_t));
}

CLCAHAL_API ErrorCreatingDeviceContextException *ErrorCreatingDeviceContextException::alloc(peff::Alloc *allocator, base::ExceptionPtr &&minorException) {
	return peff::allocAndConstruct<ErrorCreatingDeviceContextException>(allocator, sizeof(std::max_align_t), allocator, std::move(minorException));
}
#pragma endregion

#pragma region ErrorCreatingResourceException
CLCAHAL_API ErrorCreatingResourceException::ErrorCreatingResourceException(peff::Alloc *allocator, base::ExceptionPtr &&minorException) : AHALException(allocator, AHALExceptionCode::ErrorCreatingResource), minorException(std::move(minorException)) {
}

CLCAHAL_API ErrorCreatingResourceException::~ErrorCreatingResourceException() {
}

CLCAHAL_API const char *ErrorCreatingResourceException::what() const {
	return "Error creating the AHAL resource";
}

CLCAHAL_API void ErrorCreatingResourceException::dealloc() {
	peff::destroyAndRelease<ErrorCreatingResourceException>(allocator.get(), this, sizeof(std::max_align_t));
}

CLCAHAL_API ErrorCreatingResourceException *ErrorCreatingResourceException::alloc(peff::Alloc *allocator, base::ExceptionPtr &&minorException) {
	return peff::allocAndConstruct<ErrorCreatingResourceException>(allocator, sizeof(std::max_align_t), allocator, std::move(minorException));
}
#pragma endregion

#pragma region PlatformSpecificException
CLCAHAL_API PlatformSpecificException::PlatformSpecificException(peff::Alloc *allocator) : AHALException(allocator, AHALExceptionCode::PlatformSpecific) {
}

CLCAHAL_API PlatformSpecificException::~PlatformSpecificException() {
}
#pragma endregion
