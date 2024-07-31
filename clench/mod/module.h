#ifndef _CLENCH_BASE_MODULE_H_
#define _CLENCH_BASE_MODULE_H_

#include "basedefs.h"

namespace clench {
	namespace mod {
		typedef void (*ModuleInit)();
		typedef void (*ModuleDeinit)();

#if CLENCH_STATIC_LINK
		struct ModuleRegistry {
			ModuleRegistry *next;
			const char *name;
			ModuleInit moduleInit;
			ModuleDeinit moduleDeinit;
		};

		extern ModuleRegistry *g_moduleRegistries;
#elif CLENCH_IS_USER_MODULE
		extern ModuleInit g_moduleInit;
		extern ModuleDeinit g_moduleDeinit;
#endif

		struct ModuleEntry {
#if CLENCH_STATIC_LINK
			ModuleRegistry moduleRegistry;
#endif
			CLCBASE_API ModuleEntry(
				const char *name,
				ModuleInit moduleInit,
				ModuleDeinit moduleDeinit);
		};

		CLCMOD_API void initModules();
	}
}

#endif
