#include "modrt.h"

using namespace clench;
using namespace clench::mod;

ModuleEntry::ModuleEntry(
	const char *name,
	size_t nDependencies,
	const char *dependencies[],
	ModuleInit moduleInit,
	ModuleDeinit moduleDeinit,
	bool autoLoad) {
	if (!isBuiltinModulesInited()) {
		moduleRegistry.name = name;
		moduleRegistry.nDependencies = nDependencies;
		moduleRegistry.dependencies = dependencies;
		moduleRegistry.moduleInit = moduleInit;
		moduleRegistry.moduleDeinit = moduleDeinit;
		moduleRegistry.next = g_builtinModuleRegistries;
		moduleRegistry.autoLoad = autoLoad;

		g_builtinModuleRegistries = &moduleRegistry;
	} else {
		registerModule(name, nDependencies, dependencies, moduleInit, moduleDeinit);

		if (autoLoad)
			loadModule(name);
	}
}
