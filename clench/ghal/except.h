#ifndef _CLENCH_GHAL_EXCEPT_H_
#define _CLENCH_GHAL_EXCEPT_H_

#include "basedefs.h"
#include <clench/base/except.h>

namespace clench {
	namespace ghal {
		constexpr base::UUID
			EXCEPTION_TYPE_GHAL = CLENCH_UUID(0aac8fa6, 3b34, 47c4, 8f6e, 7aae71666d88);

		enum class GHALExceptionCode : uint32_t {
			NoBackendCandidate = 0,
			ErrorCreatingDeviceContext,
			InvalidContext,
			InvalidArgs,
			PlatformSpecific
		};

		class GHALException : public base::Exception {
		public:
			GHALExceptionCode ghalExceptionCode;

			CLCGHAL_API GHALException(peff::Alloc *allocator, GHALExceptionCode ghalExceptionCode);
			CLCGHAL_API virtual ~GHALException();
		};

		class ErrorCreatingDeviceContextException : public GHALException {
		public:
			base::ExceptionPtr minorException;

			CLCGHAL_API ErrorCreatingDeviceContextException(peff::Alloc *allocator, base::ExceptionPtr &&minorException);
			CLCGHAL_API virtual ~ErrorCreatingDeviceContextException();

			CLCGHAL_API const char *what() const override;
			CLCGHAL_API void dealloc() override;

			CLCGHAL_API static ErrorCreatingDeviceContextException *alloc(peff::Alloc *allocator, base::ExceptionPtr &&minorException);
		};

		class PlatformSpecificException : public GHALException {
		public:
			CLCGHAL_API PlatformSpecificException(peff::Alloc *allocator);
			CLCGHAL_API virtual ~PlatformSpecificException();
		};
	}
}

#endif
