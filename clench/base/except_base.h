#ifndef _CLENCH_BASE_EXCEPT_BASE_H_
#define _CLENCH_BASE_EXCEPT_BASE_H_

#include "uuid.h"
#include <peff/base/alloc.h>

namespace clench {
	namespace base {
		class Exception {
		public:
			mutable peff::RcObjectPtr<peff::Alloc> allocator;
			UUID typeUUID;

			CLCBASE_API Exception(peff::Alloc *allocator, const UUID &typeUUID);
			CLCBASE_API virtual ~Exception();

			virtual const char *what() const = 0;

			virtual void dealloc() = 0;
		};

		class ExceptionPointer {
		private:
			Exception *_ptr = nullptr;

		public:
			CLCBASE_API ExceptionPointer() noexcept = default;
			CLCBASE_API ExceptionPointer(Exception *exception) noexcept : _ptr(exception) {
			}

			CLCBASE_API ~ExceptionPointer() noexcept {
				unwrap();
				reset();
			}

			ExceptionPointer(const ExceptionPointer &) = delete;
			ExceptionPointer &operator=(const ExceptionPointer &) = delete;
			CLCBASE_API ExceptionPointer(ExceptionPointer &&other) noexcept {
				_ptr = other._ptr;
				other._ptr = nullptr;
			}
			CLCBASE_API ExceptionPointer &operator=(ExceptionPointer &&other) noexcept {
				_ptr = other._ptr;
				other._ptr = nullptr;
				return *this;
			}

			CLCBASE_API Exception *get() const noexcept {
				return _ptr;
			}
			CLENCH_FORCEINLINE Exception *&getRef() noexcept {
				reset();
				return _ptr;
			}
			CLENCH_FORCEINLINE Exception *&getRefWithoutRelease() noexcept {
				return _ptr;
			}
			CLENCH_FORCEINLINE Exception *const &getRefWithoutRelease() const noexcept {
				return _ptr;
			}
			CLENCH_FORCEINLINE Exception **getAddressOf() noexcept {
				reset();
				return &_ptr;
			}
			CLENCH_FORCEINLINE Exception **getAddressOfWithoutRelease() noexcept {
				return &_ptr;
			}
			CLENCH_FORCEINLINE Exception *const *getAddressOfWithoutRelease() const noexcept {
				return &_ptr;
			}

			CLCBASE_API void reset() noexcept {
				if (_ptr) {
					_ptr->dealloc();
				}
				_ptr = nullptr;
			}

			CLCBASE_API void unwrap() noexcept {
				if (_ptr) {
					assert(("Unhandled Clench internal exception: ", false));
				}
			}

			CLCBASE_API explicit operator bool() noexcept {
				return (bool)_ptr;
			}

			CLCBASE_API Exception *operator->() noexcept {
				return _ptr;
			}

			CLCBASE_API const Exception *operator->() const noexcept {
				return _ptr;
			}
		};
	}
}

#define CLENCH_RETURN_IF_EXCEPT(e) \
	if (clench::base::ExceptionPointer ptr = (e); ptr) return ptr.release();

#endif
