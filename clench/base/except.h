#ifndef _CLENCH_BASE_EXCEPT_H_
#define _CLENCH_BASE_EXCEPT_H_

#include "except_base.h"
#include <string_view>

namespace clench {
	namespace base {
		constexpr UUID
			EXCEPTION_TYPE_GENERAL = CLENCH_UUID(319ac148, d8a5, 4f4c, b984, 8f93a3c29a98);

		enum class GeneralExceptionCode : uint32_t {
			/// @brief Indicates an out of memory error.
			OutOfMemory = 0,
			/// @brief Indicates that one or more arguments are invalid.
			InvalidArgs,
		};

		class GeneralException : public Exception {
		public:
			GeneralExceptionCode errorCode;

			CLCBASE_API GeneralException(peff::Alloc *allocator, GeneralExceptionCode errorCode);
			CLCBASE_API virtual ~GeneralException();
		};

		class OutOfMemoryException final : public GeneralException {
		public:
			CLCBASE_API OutOfMemoryException();
			CLCBASE_API virtual ~OutOfMemoryException();

			CLCBASE_API virtual const char *what() const override;

			CLCBASE_API virtual void dealloc() override;

			CLCBASE_API static OutOfMemoryException *alloc();
		};

		extern OutOfMemoryException g_outOfMemoryException;

		class InvalidArgsException final : public GeneralException {
		public:
			CLCBASE_API InvalidArgsException(peff::Alloc *allocator);
			CLCBASE_API virtual ~InvalidArgsException();

			CLCBASE_API virtual const char *what() const override;

			CLCBASE_API virtual void dealloc() override;

			CLCBASE_API static InvalidArgsException *alloc(peff::Alloc *allocator);
		};

		PEFF_FORCEINLINE ExceptionPointer wrapIfExceptAllocFailed(Exception *exceptionPtr) {
			if(!exceptionPtr)
				return OutOfMemoryException::alloc();
			return exceptionPtr;
		}
	}
}

#endif
