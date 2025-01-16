#ifndef _CLENCH_BASE_EXCEPT_H_
#define _CLENCH_BASE_EXCEPT_H_

#include "uuid.h"
#include <peff/base/alloc.h>

namespace clench {
	namespace base {
		constexpr UUID
			EXCEPTION_TYPE_GENERAL = CLENCH_UUID(319ac148, d8a5, 4f4c, b984, 8f93a3c29a98);

		class InternalException {
		public:
			mutable peff::RcObjectPtr<peff::Alloc> allocator;
			UUID typeUUID;

			CLCBASE_API InternalException(peff::Alloc *allocator, const UUID &typeUUID);
			CLCBASE_API virtual ~InternalException();

			virtual const char *what() const = 0;

			virtual void dealloc() = 0;
		};

		class InternalExceptionPointer {
		private:
			InternalException *_ptr = nullptr;

		public:
			CLCBASE_API InternalExceptionPointer() noexcept = default;
			CLCBASE_API InternalExceptionPointer(InternalException *exception) noexcept : _ptr(exception) {
			}

			CLCBASE_API ~InternalExceptionPointer() noexcept {
				unwrap();
				reset();
			}

			InternalExceptionPointer(const InternalExceptionPointer &) = delete;
			InternalExceptionPointer &operator=(const InternalExceptionPointer &) = delete;
			CLCBASE_API InternalExceptionPointer(InternalExceptionPointer &&other) noexcept {
				_ptr = other._ptr;
				other._ptr = nullptr;
			}
			CLCBASE_API InternalExceptionPointer &operator=(InternalExceptionPointer &&other) noexcept {
				_ptr = other._ptr;
				other._ptr = nullptr;
				return *this;
			}

			CLCBASE_API InternalException *get() noexcept {
				return _ptr;
			}
			CLCBASE_API const InternalException *get() const noexcept {
				return _ptr;
			}

			CLCBASE_API void reset() noexcept {
				if (_ptr) {
					_ptr->dealloc();
				}
				_ptr = nullptr;
			}

			CLCBASE_API void unwrap() noexcept {
				if (_ptr) {
					assert(("Unhandled WandXML internal exception: ", false));
				}
			}

			CLCBASE_API explicit operator bool() noexcept {
				return (bool)_ptr;
			}

			CLCBASE_API InternalException *operator->() noexcept {
				return _ptr;
			}

			CLCBASE_API const InternalException *operator->() const noexcept {
				return _ptr;
			}
		};
	}
}

#endif
