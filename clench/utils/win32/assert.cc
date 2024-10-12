#include <clench/utils/assert.h>
#include <Windows.h>

using namespace clench;
using namespace clench::utils;

#include <cstdio>
#include <cstdlib>
#include <malloc.h>

CLCUTILS_API void clench::utils::defaultAssertionHandler(const char *file, size_t line, const char *failMessage) {
	size_t size = snprintf(nullptr, 0, "Assertion failed at %s, line %zu: %s", file, line, failMessage);

	char *message = (char *)alloca(size + 1);

	sprintf(message, "Assertion failed at %s, line %zu: %s", file, line, failMessage);

	MessageBox(nullptr, message, "Assertion Failed", MB_OK | MB_ICONERROR);

	abort();
}
