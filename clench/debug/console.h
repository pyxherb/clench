#ifndef _CLENCH_DEBUG_CONSOLE_H_
#define _CLENCH_DEBUG_CONSOLE_H_

#include <csignal>
#include "log.hh"

namespace clench {
	namespace debug {
		void debugConsoleHandler(int signal);

		inline void setupDebugConsole() {
			#ifdef _DEBUG
			std::signal(SIGTERM, SIG_IGN);
			std::signal(SIGINT, debugConsoleHandler);
			#endif
		}
	}
}

#endif
