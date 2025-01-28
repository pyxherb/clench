#ifndef _CLENCH_WSAL_EXCEPT_H_
#define _CLENCH_WSAL_EXCEPT_H_

#include <clench/base/except.h>

namespace clench {
	namespace wsal {
		constexpr base::UUID
			EXCEPTION_TYPE_WSAL = CLENCH_UUID(d0049276, b769, 44f7, bb3e, d1f7d0a16df2);

		enum class WSALExceptionCode : uint32_t {
			/// @brief Indicates that the display was failed to be opened.
			ErrorOpeningDisplay,
			/// @brief Indicates that the creation of the window was failed.
			ErrorCreatingWindow,
		};

		class WSALException : public base::Exception {
		public:
			WSALExceptionCode wsalExceptionCode;

			CLCBASE_API WSALException(peff::Alloc *allocator, WSALExceptionCode wsalExceptionCode);
			CLCBASE_API virtual ~WSALException();
		};

		class ErrorOpeningDisplayException : public WSALException {
		public:
			CLCBASE_API ErrorOpeningDisplayException(peff::Alloc *allocator);
			CLCBASE_API virtual ~ErrorOpeningDisplayException();

			CLCBASE_API virtual const char *what() const override;

			CLCBASE_API virtual void dealloc() override;

			CLCBASE_API static ErrorOpeningDisplayException *alloc(peff::Alloc *allocator);
		};

		class ErrorCreatingWindowException : public WSALException {
		public:
			base::ExceptionPtr minorException;

			CLCBASE_API ErrorCreatingWindowException(peff::Alloc *allocator, base::Exception *minorException);
			CLCBASE_API virtual ~ErrorCreatingWindowException();

			CLCBASE_API virtual const char *what() const override;

			CLCBASE_API virtual void dealloc() override;

			CLCBASE_API static ErrorCreatingWindowException *alloc(peff::Alloc *allocator, base::Exception *minorException);
		};
	}
}

#endif
