#include "assert.h"

using namespace clench;
using namespace clench::utils;

AssertionHandler clench::utils::g_assertionHandler = defaultAssertionHandler;

CLCUTILS_API void clench::utils::doAssert(bool condition, const char *file, size_t line, const char *failMessage) {
	if (!condition) {
		g_assertionHandler(file, line, failMessage);
	}
}
