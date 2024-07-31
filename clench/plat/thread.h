#ifndef _CLENCH_UTILS_THREAD_H_
#define _CLENCH_UTILS_THREAD_H_

#include "basedefs.h"

#include <cstdint>
#include <stdexcept>

#if defined(_WIN32)

	#include <Windows.h>

#elif __linux__

	#include <sched.h>

#endif

namespace clench {
	namespace plat {
		using ThreadID = uint32_t;

		class Runnable {
		public:
			CLCPLAT_API virtual ~Runnable();
			virtual void run() = 0;
		};

		enum class ThreadState : uint8_t {
			Ready = 0,
			Running,
			Suspended,
			Done,
		};

		class Thread {
		private:
			ThreadState _state = ThreadState::Ready;

#if defined(_WIN32)
			CLCPLAT_API static DWORD WINAPI _win32ThreadWrapper(LPVOID param);
#elif defined(__linux__)
			CLCPLAT_API static int _linuxThreadWrapper(void *param);
			char *_threadStack = nullptr;
#endif

		public:
			Runnable *runnable;
			const size_t stackSize;
#if defined(_WIN32)
			HANDLE nativeHandle = INVALID_HANDLE_VALUE;
#elif defined(__linux__)
			int nativeHandle = -1;
#endif
			CLCPLAT_API Thread(Runnable *runnable, size_t stackSize);
			CLCPLAT_API virtual ~Thread();

			CLCPLAT_API void start();

			FORCEINLINE ThreadState getState() {
				return _state;
			}
		};

		CLCPLAT_API void yield();

		enum class AsyncPolicy {
			Immediate = 0,
			Lazy
		};

		enum class FutureState {
			Pending = 0,
			Resolved,
			Rejected
		};

		template <typename T>
		class Future {
		public:
			FutureState state;
			T result;
			std::exception_ptr exception;
		};

		template <typename T>
		class Promise {
		public:
			Future<T> *const future;

			FORCEINLINE Promise(Future<T> *future) : future(future) {}
			FORCEINLINE void resolve(T &&result) {
				future->result = result;
				future->state = FutureState::Resolved;
			}
			FORCEINLINE void reject(std::exception_ptr exception) {
				future->exception = exception;
				future->state = FutureState::Rejected;
			}
		};
	}
}

#endif
