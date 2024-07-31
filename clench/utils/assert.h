#ifndef _CLENCH_UTILS_ASSERT_H_
#define _CLENCH_UTILS_ASSERT_H_

#include "basedefs.h"

namespace clench {
	namespace utils {
		typedef void (*AssertionHandler)(const char *file, size_t line, const char *failMessage);

		CLCUTILS_API void defaultAssertionHandler(const char *file, size_t line, const char *failMessage);

		extern AssertionHandler g_assertionHandler;

		CLCUTILS_API void doAssert(bool condition, const char *file, size_t line, const char *failMessage);
	}
}

#ifdef NDEBUG

	#define CLENCH_ASSERT(cond, failMessage)

#else

	#define CLENCH_ASSERT(cond, failMessage) \
		clench::utils::doAssert(cond, __FILE__, __LINE__, failMessage)

#endif

#endif
