#include "thread.h"

using namespace clench;
using namespace clench::plat;

CLCPLAT_API Runnable::~Runnable() {}

CLCPLAT_API Thread::Thread(Runnable *runnable, size_t stackSize)
	: runnable(runnable), stackSize(stackSize) {
}

CLCPLAT_API Thread::~Thread() {
}
