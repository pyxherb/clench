#ifndef _CLENCH_AHAL_EXCEPT_H_
#define _CLENCH_AHAL_EXCEPT_H_

#include "basedefs.h"
#include <peff/containers/string.h>
#include <clench/base/except.h>

namespace clench {
	namespace ahal {
		constexpr base::UUID
			EXCEPTION_TYPE_AHAL = CLENCH_UUID(18f2e1f8, fde8, 4177, 9dce, 8307767d2986);

		enum class AHALExceptionCode : uint32_t {
			NoBackendCandidate = 0,
			ErrorOpeningDevice,
			ErrorCreatingDeviceContext,
			ErrorCreatingResource,
			PlatformSpecific
		};

		class AHALException : public base::Exception {
		public:
			AHALExceptionCode ahalExceptionCode;

			CLCAHAL_API AHALException(peff::Alloc *allocator, AHALExceptionCode ahalExceptionCode);
			CLCAHAL_API virtual ~AHALException();
		};

		class ErrorOpeningDeviceException : public AHALException {
		public:
			base::ExceptionPtr minorException;

			CLCAHAL_API ErrorOpeningDeviceException(peff::Alloc *allocator);
			CLCAHAL_API virtual ~ErrorOpeningDeviceException();

			CLCAHAL_API const char *what() const override;
			CLCAHAL_API void dealloc() override;

			CLCAHAL_API static ErrorOpeningDeviceException *alloc(peff::Alloc *allocator);
		};

		class ErrorCreatingDeviceContextException : public AHALException {
		public:
			base::ExceptionPtr minorException;

			CLCAHAL_API ErrorCreatingDeviceContextException(peff::Alloc *allocator, base::ExceptionPtr &&minorException);
			CLCAHAL_API virtual ~ErrorCreatingDeviceContextException();

			CLCAHAL_API const char *what() const override;
			CLCAHAL_API void dealloc() override;

			CLCAHAL_API static ErrorCreatingDeviceContextException *alloc(peff::Alloc *allocator, base::ExceptionPtr &&minorException);
		};

		class ErrorCreatingResourceException : public AHALException {
		public:
			base::ExceptionPtr minorException;

			CLCAHAL_API ErrorCreatingResourceException(peff::Alloc *allocator, base::ExceptionPtr &&minorException);
			CLCAHAL_API virtual ~ErrorCreatingResourceException();

			CLCAHAL_API const char *what() const override;
			CLCAHAL_API void dealloc() override;

			CLCAHAL_API static ErrorCreatingResourceException *alloc(peff::Alloc *allocator, base::ExceptionPtr &&minorException);
		};

		class PlatformSpecificException : public AHALException {
		public:
			CLCAHAL_API PlatformSpecificException(peff::Alloc *allocator);
			CLCAHAL_API virtual ~PlatformSpecificException();
		};
	}
}

#endif
