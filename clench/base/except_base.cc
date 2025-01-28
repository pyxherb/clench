#include "except_base.h"

using namespace clench;
using namespace clench::base;

CLCBASE_API Exception::Exception(peff::Alloc *allocator, const UUID &typeUUID): allocator(allocator), typeUUID(typeUUID) {
}

CLCBASE_API Exception::~Exception() {
}
