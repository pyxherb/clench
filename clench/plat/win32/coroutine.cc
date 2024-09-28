#include "../coroutine.h"
#include <clench/utils/assert.h>

using namespace clench;
using namespace clench::plat;

CLCPLAT_API std::map<std::thread::id, CoroutineTlsInfo> clench::plat::g_coroutineTlsInfo;

CLCPLAT_API void WINAPI NativeCoroutine::_win32Wrapper(LPVOID param) {
	NativeCoroutine *coroutine = (NativeCoroutine *)param;

	coroutine->runnable->run();

	auto it = g_coroutineTlsInfo.find(std::this_thread::get_id());

	it->second.curCoroutine->state = ThreadState::Done;

	auto &tlsData = it->second;

	if (tlsData.awaitList.size()) {
		tlsData.awaitList.back()->resume();
	} else {
		SwitchToFiber(tlsData.threadFiber);
	}
}

CLCPLAT_API NativeCoroutine::NativeCoroutine(size_t stackSize, Runnable *runnable) : stackSize(stackSize), runnable(runnable) {
	nativeHandle = CreateFiberEx(0, stackSize, FIBER_FLAG_FLOAT_SWITCH, _win32Wrapper, this);
}

CLCPLAT_API NativeCoroutine::~NativeCoroutine() {
	DeleteFiber(nativeHandle);
}

CLCPLAT_API void NativeCoroutine::resume() {
	auto it = g_coroutineTlsInfo.find(std::this_thread::get_id());

	if (it != g_coroutineTlsInfo.end()) {
		it->second.awaitList.push_back(it->second.curCoroutine);
		it->second.curCoroutine = this;

		state = ThreadState::Running;

		SwitchToFiber(nativeHandle);

		it->second.awaitList.pop_back();
	} else {
		auto &tlsInfo = g_coroutineTlsInfo[std::this_thread::get_id()];

		tlsInfo.threadFiber = ConvertThreadToFiberEx(nullptr, FIBER_FLAG_FLOAT_SWITCH);

		tlsInfo.curCoroutine = this;
		state = ThreadState::Running;

		SwitchToFiber(nativeHandle);

		g_coroutineTlsInfo.erase(std::this_thread::get_id());

		ConvertFiberToThread();
	}
}

CLCPLAT_API void clench::plat::yieldNativeCoroutine() {
	auto it = g_coroutineTlsInfo.find(std::this_thread::get_id());

	CLENCH_ASSERT(it != g_coroutineTlsInfo.end(), "");

	it->second.curCoroutine->state = ThreadState::Suspended;

	auto &tlsData = it->second;

	if (tlsData.awaitList.size()) {
		tlsData.awaitList.back()->resume();
	} else {
		SwitchToFiber(tlsData.threadFiber);
	}
}
