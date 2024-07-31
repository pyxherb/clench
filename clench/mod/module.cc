#include "module.h"

using namespace clench;
using namespace clench::mod;

#if CLENCH_STATIC_LINK
ModuleRegistry *clench::base::g_moduleRegistries = nullptr;
#elif CLENCH_IS_USER_MODULE
ModuleInit clench::base::g_moduleInit;
ModuleDeinit clench::base::g_moduleDeinit;
#endif

CLCBASE_API ModuleEntry::ModuleEntry(
	const char *name,
	ModuleInit moduleInit,
	ModuleDeinit moduleDeinit) {
#if CLENCH_STATIC_LINK
	moduleRegistry.name = name;
	moduleRegistry.moduleInit = moduleInit;
	moduleRegistry.moduleDeinit = moduleDeinit;
	moduleRegistry.next = g_moduleRegistries;
	g_moduleRegistries = &moduleRegistry;
#elif CLENCH_IS_USER_MODULE
	g_moduleInit = moduleInit;
	g_moduleDeinit = moduleDeinit;
#endif
}

void clench::mod::initModules() {
#if CLENCH_STATIC_LINK
#endif
}
