#include "modrt.h"

using namespace clench;
using namespace clench::mod;

#if _WIN32
static HMODULE _getCurrentModule();
#endif

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
		moduleRegistry.moduleRegisterCounter = g_moduleRegisterCounter++;

#ifdef _WIN32
		moduleRegistry.nativeHandle = _getCurrentModule();
#endif

		g_builtinModuleRegistries = &moduleRegistry;
	} else {
		Module *pModule = registerModule(name, nDependencies, dependencies, moduleInit, moduleDeinit);

#ifdef _WIN32
		pModule->nativeHandle = _getCurrentModule();
#endif

		if (pModule) {
			if (autoLoad)
				loadModule(name);
		}
	}
}

#if _WIN32
static HMODULE _getCurrentModule() {
	HMODULE hModule;
	GetModuleHandleEx(
		GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
		(LPCTSTR)_getCurrentModule,
		&hModule);
	return hModule;
}
#endif
