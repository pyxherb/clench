#include <clench/utils/assert.h>

using namespace clench;
using namespace clench::utils;

#include <cstdio>
#include <cstdlib>

CLCUTILS_API void clench::utils::defaultAssertionHandler(const char *file, size_t line, const char *failMessage) {
	printf("Assertion failed at %s, line %zu: %s\n", file, line, failMessage);

	abort();
}
