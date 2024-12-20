#ifndef _CLENCH_UTILS_RCOBJ_H_
#define _CLENCH_UTILS_RCOBJ_H_

#include "basedefs.h"
#include <atomic>
#include <cstdint>
#include <type_traits>
#include <mutex>

namespace clench {
	namespace utils {
		class RcObject {
		public:
			std::atomic_size_t refCount = 0;

			CLENCH_NO_COPY_MOVE_METHODS(RcObject);

			CLCUTILS_API RcObject();
			CLCUTILS_API virtual ~RcObject();

			CLCUTILS_API virtual void onRefZero();

			CLENCH_FORCEINLINE size_t incRef() noexcept {
				return ++refCount;
			}

			CLENCH_FORCEINLINE size_t decRef() {
				if (!(--refCount))
					onRefZero();
				return refCount;
			}
		};

		template <typename T>
		class RcObjectPtr {
		private:
			T *_ptr = nullptr;

			CLENCH_FORCEINLINE void _setAndIncRef(T *_ptr) {
				this->_ptr = _ptr;
				_ptr->incRef();
			}

		public:
			CLENCH_FORCEINLINE void reset() {
				if (_ptr)
					_ptr->decRef();
				_ptr = nullptr;
			}

			CLENCH_FORCEINLINE RcObjectPtr(T *ptr = nullptr) {
				if (ptr)
					_setAndIncRef(ptr);
			}
			CLENCH_FORCEINLINE RcObjectPtr(const RcObjectPtr<T> &other) {
				_setAndIncRef(other._ptr);
			}
			CLENCH_FORCEINLINE RcObjectPtr(RcObjectPtr<T> &&other) {
				_ptr = other._ptr;
				other._ptr = nullptr;
			}
			CLENCH_FORCEINLINE ~RcObjectPtr() {
				reset();
			}

			CLENCH_FORCEINLINE RcObjectPtr<T> &operator=(T *_ptr) noexcept {
				reset();
				_setAndIncRef(_ptr);
				return *this;
			}
			CLENCH_FORCEINLINE RcObjectPtr<T> &operator=(const RcObjectPtr<T> &other) noexcept {
				reset();
				_setAndIncRef(other._ptr);
				return *this;
			}
			CLENCH_FORCEINLINE RcObjectPtr<T> &operator=(RcObjectPtr<T> &&other) noexcept {
				reset();
				_ptr = other._ptr;
				other._ptr = nullptr;

				return *this;
			}

			CLENCH_FORCEINLINE T *get() {
				return _ptr;
			}
			CLENCH_FORCEINLINE T *operator->() {
				return _ptr;
			}

			CLENCH_FORCEINLINE bool operator<(const RcObjectPtr<T> &rhs) const {
				return _ptr < rhs._ptr;
			}

			CLENCH_FORCEINLINE operator bool() const {
				return _ptr;
			}
		};

		template<typename T>
		struct RcObjectUniquePtrDeleter {
			CLENCH_FORCEINLINE void operator()(T *ptr) {
				ptr->onRefZero();
			}
		};
	}
}

#endif
