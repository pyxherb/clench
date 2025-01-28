#include "except.h"

using namespace clench;
using namespace clench::wsal;

#pragma region WSALException
CLCWSAL_API WSALException::WSALException(peff::Alloc *allocator, WSALExceptionCode wsalExceptionCode) : Exception(allocator, EXCEPTION_TYPE_WSAL), wsalExceptionCode(wsalExceptionCode) {
}

CLCWSAL_API WSALException::~WSALException() {
}
#pragma endregion

#pragma region ErrorOpeningDisplayException
CLCWSAL_API ErrorOpeningDisplayException::ErrorOpeningDisplayException(peff::Alloc *allocator) : WSALException(allocator, WSALExceptionCode::ErrorOpeningDisplay) {
}

CLCWSAL_API ErrorOpeningDisplayException::~ErrorOpeningDisplayException() {
}

CLCWSAL_API const char *ErrorOpeningDisplayException::what() const {
	return "Error opening the display";
}

CLCWSAL_API void ErrorOpeningDisplayException::dealloc() {
	peff::destroyAndRelease<ErrorOpeningDisplayException>(allocator.get(), this, sizeof(std::max_align_t));
}

CLCWSAL_API ErrorOpeningDisplayException *ErrorOpeningDisplayException::alloc(peff::Alloc *allocator) {
	return peff::allocAndConstruct<ErrorOpeningDisplayException>(allocator, sizeof(std::max_align_t), allocator);
}
#pragma endregion

#pragma region ErrorCreatingWindowException
CLCWSAL_API ErrorCreatingWindowException::ErrorCreatingWindowException(peff::Alloc *allocator, base::ExceptionPtr &&minorException) : WSALException(allocator, WSALExceptionCode::ErrorCreatingWindow), minorException(std::move(minorException)) {
}

CLCWSAL_API ErrorCreatingWindowException::~ErrorCreatingWindowException() {
}

CLCWSAL_API const char *ErrorCreatingWindowException::what() const {
	return "Error opening the display";
}

CLCWSAL_API void ErrorCreatingWindowException::dealloc() {
	peff::destroyAndRelease<ErrorCreatingWindowException>(allocator.get(), this, sizeof(std::max_align_t));
}

CLCWSAL_API ErrorCreatingWindowException *ErrorCreatingWindowException::alloc(peff::Alloc *allocator, base::ExceptionPtr &&minorException) {
	return peff::allocAndConstruct<ErrorCreatingWindowException>(allocator, sizeof(std::max_align_t), allocator, std::move(minorException));
}
#pragma endregion
