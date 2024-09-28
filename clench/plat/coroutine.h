#ifndef _CLENCH_PLAT_COROUTINE_H_
#define _CLENCH_PLAT_COROUTINE_H_

#include "thread.h"
#include <list>
#include <map>
#include <thread>

namespace clench {
	namespace plat {
		class Coroutine {
		public:
			ThreadState state = ThreadState::Ready;

			CLCPLAT_API virtual ~Coroutine();

			virtual void resume() = 0;
		};

		class NativeCoroutine : public Coroutine {
		public:
#ifdef _WIN32
			CLCPLAT_API static void WINAPI _win32Wrapper(LPVOID param);
			LPVOID nativeHandle;
#endif

			const size_t stackSize;
			Runnable *runnable;

			CLCPLAT_API NativeCoroutine(size_t stackSize, Runnable *runnable);
			CLCPLAT_API virtual ~NativeCoroutine();

			CLCPLAT_API virtual void resume();
		};

		struct CoroutineTlsInfo {
			LPVOID threadFiber;
			Coroutine *curCoroutine;
			std::list<Coroutine *> awaitList;
		};

		CLCPLAT_API extern std::map<std::thread::id, CoroutineTlsInfo> g_coroutineTlsInfo;

		CLCPLAT_API void yieldNativeCoroutine();
	}
}

#define CLENCH_YIELD clench::plat::yieldNativeCoroutine()

#endif
