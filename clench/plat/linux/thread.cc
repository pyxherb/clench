#include <clench/plat/thread.h>

using namespace clench;
using namespace clench::plat;

CLCPLAT_API void clench::plat::yield() {
	sched_yield();
}

CLCPLAT_API int Thread::_linuxThreadWrapper(void *param) {
	Thread *thread = (Thread *)param;

	thread->_state = ThreadState::Running;

	thread->runnable->run();

	thread->_state = ThreadState::Done;

	return 0;
}

CLCPLAT_API void Thread::start() {
	nativeHandle = clone(_linuxThreadWrapper,
		_threadStack = new char[stackSize],
		0,
		this);
}

CLCPLAT_API Thread::~Thread() {
	if (_threadStack)
		delete[] _threadStack;
}
