#include "except.h"

using namespace clench;
using namespace clench::base;

OutOfMemoryException clench::base::g_outOfMemoryException;

#pragma region OutOfMemoryException
CLCBASE_API OutOfMemoryException::OutOfMemoryException() : GeneralException(nullptr, GeneralExceptionCode::OutOfMemory) {
}

CLCBASE_API OutOfMemoryException::~OutOfMemoryException() {
}

CLCBASE_API const char *OutOfMemoryException::what() const {
	return "Out of memory";
}

CLCBASE_API void OutOfMemoryException::dealloc() {
}

CLCBASE_API OutOfMemoryException *OutOfMemoryException::alloc() {
	return &g_outOfMemoryException;
}
#pragma endregion

#pragma region GeneralException

CLCBASE_API GeneralException::GeneralException(peff::Alloc *allocator, GeneralExceptionCode errorCode) : Exception(allocator, EXCEPTION_TYPE_GENERAL), errorCode(errorCode) {
}

CLCBASE_API GeneralException::~GeneralException() {
}

#pragma endregion

#pragma region InvalidArgsException
CLCBASE_API InvalidArgsException::InvalidArgsException(peff::Alloc *allocator) : GeneralException(allocator, GeneralExceptionCode::InvalidArgs) {
}

CLCBASE_API InvalidArgsException::~InvalidArgsException() {
}

CLCBASE_API const char *InvalidArgsException::what() const {
	return "Invalid arguments";
}

CLCBASE_API void InvalidArgsException::dealloc() {
	peff::destroyAndRelease<InvalidArgsException>(allocator.get(), this, sizeof(std::max_align_t));
}

CLCBASE_API InvalidArgsException *InvalidArgsException::alloc(peff::Alloc *allocator) {
	return peff::allocAndConstruct<InvalidArgsException>(allocator, sizeof(std::max_align_t), allocator);
}
#pragma endregion
