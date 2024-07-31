#include "rcobj.h"

using namespace clench;
using namespace clench::utils;

CLCUTILS_API RcObject::RcObject() {}
CLCUTILS_API RcObject::~RcObject() {}

CLCUTILS_API void RcObject::onRefZero() {
	delete this;
}