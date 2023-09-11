#ifndef _CLENCH_DEBUG_MM_HG_
#define _CLENCH_DEBUG_MM_HG_

// Memory leak detection
#ifndef NDEBUG
	#ifdef _WIN32
		#define _CRTDBG_MAP_ALLOC
		#include <crtdbg.h>
		#include <cstdlib>
		#define malloc(x) _malloc_dbg(x, _NORMAL_BLOCK, __FILE__, __LINE__)
		#define free(x) _free_dbg(x, _NORMAL_BLOCK)
		#define __DBG_NEW new (_NORMAL_BLOCK, __FILE__, __LINE__)
		#define new __DBG_NEW

	#endif
#endif

namespace clench {
	namespace debug {
		void inline setupMemoryLeakDetector() noexcept {
#if defined(_WIN32) && defined(_MSC_VER) && defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
		}

		void inline dumpMemoryLeaks() noexcept {
#if defined(_WIN32) && defined(_MSC_VER) && defined(_DEBUG)
			_CrtDumpMemoryLeaks();
#endif
		}
	}
}

#endif
