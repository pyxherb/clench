#include "except.h"

using namespace clench;
using namespace clench::base;

CLCBASE_API InternalException::InternalException(peff::Alloc *allocator, const UUID &typeUUID): allocator(allocator), typeUUID(typeUUID) {
}

CLCBASE_API InternalException::~InternalException() {
}
