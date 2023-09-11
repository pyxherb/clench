#ifndef _CLENCH_CLENCH_H_
#define _CLENCH_CLENCH_H_

#include <cstdint>

#include "debug.h"
#include "graphics.h"
#include "input.h"
#include "math.h"

namespace clench {
	using InitFlags = std::uint32_t;
	void init(InitFlags flags);
	void deinit();

	int gameMain(int argc, char* argv[]);
}

#endif
