#ifndef _CLENCH_MODRT_MODRT_H_
#define _CLENCH_MODRT_MODRT_H_

#include <clench/mod/module.h>

namespace clench {
	namespace mod {
		struct ModuleEntry {
			BuiltinModuleRegistry moduleRegistry;

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
