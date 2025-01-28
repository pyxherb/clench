#include "except.h"

using namespace clench;
using namespace clench::wsal;

#pragma region WSALException
CLCBASE_API WSALException::WSALException(peff::Alloc *allocator, WSALExceptionCode wsalExceptionCode) : Exception(allocator, EXCEPTION_TYPE_WSAL), wsalExceptionCode(wsalExceptionCode) {
}

CLCBASE_API WSALException::~WSALException() {
}
#pragma endregion

#pragma region ErrorOpeningDisplayException
CLCBASE_API ErrorOpeningDisplayException::ErrorOpeningDisplayException(peff::Alloc *allocator) : WSALException(allocator, WSALExceptionCode::ErrorOpeningDisplay) {
}

CLCBASE_API ErrorOpeningDisplayException::~ErrorOpeningDisplayException() {
}

CLCBASE_API const char *ErrorOpeningDisplayException::what() const {
	return "Error opening the display";
}

CLCBASE_API void ErrorOpeningDisplayException::dealloc() {
}

CLCBASE_API ErrorOpeningDisplayException *ErrorOpeningDisplayException::alloc(peff::Alloc *allocator) {
	return peff::allocAndConstruct<ErrorOpeningDisplayException>(allocator, sizeof(std::max_align_t), allocator);
}
#pragma endregion

#pragma region ErrorCreatingWindowException
CLCBASE_API ErrorCreatingWindowException::ErrorCreatingWindowException(peff::Alloc *allocator, base::Exception *minorException) : WSALException(allocator, WSALExceptionCode::ErrorCreatingWindow), minorException(minorException) {
}

CLCBASE_API ErrorCreatingWindowException::~ErrorCreatingWindowException() {
}

CLCBASE_API const char *ErrorCreatingWindowException::what() const {
	return "Error opening the display";
}

CLCBASE_API void ErrorCreatingWindowException::dealloc() {
}

CLCBASE_API ErrorCreatingWindowException *ErrorCreatingWindowException::alloc(peff::Alloc *allocator, base::Exception *minorException) {
	return peff::allocAndConstruct<ErrorCreatingWindowException>(allocator, sizeof(std::max_align_t), allocator, minorException);
}
#pragma endregion
