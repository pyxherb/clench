#ifndef _CLENCH_MODRT_MODRT_H_
#define _CLENCH_MODRT_MODRT_H_

#include <clench/mod/module.h>

namespace clench {
	namespace mod {
		extern const char *g_moduleName;
		extern ModuleInit g_moduleInit;
		extern ModuleDeinit g_moduleDeinit;

		struct ModuleEntry {
#if CLENCH_STATIC_LINK
			BuiltinModuleRegistry moduleRegistry;
#endif

			ModuleEntry(
				const char *name,
				size_t nDependencies,
				const char *dependencies[],
				ModuleInit moduleInit,
				ModuleDeinit moduleDeinit,
				bool autoLoad = false);
		};
	}
}

#endif
