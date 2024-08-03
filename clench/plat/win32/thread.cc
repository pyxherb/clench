#include <clench/plat/thread.h>
#include <clench/utils/assert.h>

using namespace clench;
using namespace clench::plat;

CLCPLAT_API DWORD WINAPI clench::plat::Thread::_win32ThreadWrapper(LPVOID param) {
	Thread *thread = (Thread *)param;

	thread->_state = ThreadState::Running;

	thread->runnable->run();

	thread->_state = ThreadState::Done;

	return 0;
}

CLCPLAT_API Thread::~Thread() {
}

CLCPLAT_API void Thread::start() {
	nativeHandle = CreateThread(nullptr, stackSize, _win32ThreadWrapper, this, 0, 0);
}

CLCPLAT_API void clench::plat::yield() {
	Sleep(0);
}
