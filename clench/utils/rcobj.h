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

			NO_COPY_MOVE_METHODS(RcObject);

			CLCUTILS_API RcObject();
			CLCUTILS_API virtual ~RcObject();

			CLCUTILS_API virtual void onRefZero();

			FORCEINLINE size_t incRef() noexcept {
				return ++refCount;
			}

			FORCEINLINE size_t decRef() {
				if (!(--refCount))
					onRefZero();
				return refCount;
			}
		};

		template<typename T>
		class RcObjectPtr {
		private:
			T* _ptr;

			FORCEINLINE void _setAndIncRef(T* _ptr) {
				this->_ptr = _ptr;
				_ptr->incRef();
			}

		public:

			FORCEINLINE void reset() {
				if (_ptr)
					_ptr->decRef();
				_ptr = nullptr;
			}

			FORCEINLINE RcObjectPtr(T* _ptr = nullptr) : _ptr(_ptr) {}
			FORCEINLINE RcObjectPtr(const RcObjectPtr<T>& other) {
				_setAndIncRef(_ptr);
			}
			FORCEINLINE ~RcObjectPtr() {
				reset();
			}

			FORCEINLINE RcObjectPtr<T>& operator=(T* _ptr) noexcept {
				reset();
				_setAndIncRef(_ptr);
				return *this;
			}
			FORCEINLINE RcObjectPtr<T>& operator=(const RcObjectPtr<T>& other) noexcept {
				reset();
				_setAndIncRef(other._ptr);
				return *this;
			}
			FORCEINLINE RcObjectPtr<T>& operator=(RcObjectPtr<T>&& other) noexcept {
				reset();
				_ptr = other._ptr;
				other._ptr = nullptr;

				return *this;
			}

			FORCEINLINE T* get() {
				return _ptr;
			}
			FORCEINLINE T* operator->() {
				return _ptr;
			}
		};
	}
}

#endif