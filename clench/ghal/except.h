#ifndef _CLENCH_GHAL_EXCEPT_H_
#define _CLENCH_GHAL_EXCEPT_H_

#include <clench/base/except.h>

namespace clench {
	namespace ghal {
		constexpr base::UUID
			EXCEPTION_TYPE_GHAL = CLENCH_UUID(0aac8fa6, 3b34, 47c4, 8f6e, 7aae71666d88);

		enum class GHALExceptionCode : uint32_t {

		};

		class GHALException {
		public:
			CLCBASE_API GHALException(peff::Alloc *allocator);
			CLCBASE_API virtual ~GHALException();

			virtual const char *what() const = 0;

			virtual void dealloc() = 0;
		};
	}
}

#endif
