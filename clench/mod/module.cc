#include "module.h"

using namespace clench;
using namespace clench::mod;

CLCMOD_API BuiltinModuleRegistry *clench::mod::g_builtinModuleRegistries = nullptr;
CLCMOD_API std::unordered_map<std::string, std::unique_ptr<Module>> clench::mod::g_registeredModules;

static bool _g_isBuiltinModulesInited = false;

CLCMOD_API Module::Module(
	const char *name,
	ModuleInit moduleInit,
	ModuleDeinit moduleDeinit,
	bool isBuiltin) : name(name),
					  moduleInit(moduleInit),
					  moduleDeinit(moduleDeinit),
					  isBuiltin(isBuiltin) {
}

CLCMOD_API Module::~Module() {
	if (isInited)
		moduleDeinit();
}

CLCMOD_API bool clench::mod::initBuiltinModules() {
	std::unordered_set<std::string> autoLoadModuleNames;

	for (BuiltinModuleRegistry *i = g_builtinModuleRegistries; i; i = i->next) {
		registerModule(
			i->name,
			i->nDependencies, i->dependencies,
			i->moduleInit, i->moduleDeinit,
			true);

		if (i->autoLoad) {
			autoLoadModuleNames.insert(i->name);
		}
	}

	for (auto i : autoLoadModuleNames) {
		if (!loadModule(i.c_str())) {
			deinitModules();
			return false;
		}
	}

	_g_isBuiltinModulesInited = true;

	return true;
}

CLCMOD_API void clench::mod::deinitModules() {
	std::unordered_set<std::string> unusedModules;

	for (auto &i : g_registeredModules) {
		if (i.second->isReleasable()) {
			CLENCH_WARNING_LOG("Mod", "Found unused module: %s", i.first.c_str());
		} else if (i.second->isManuallyLoaded) {
			unloadModule(i.first.c_str());
		}
	}

#ifndef NDEBUG
	for (auto &i : g_registeredModules) {
		CLENCH_ASSERT(!i.second->refCount, "Found modules that still keep loaded");
	}
#endif

	g_registeredModules.clear();
}

CLCMOD_API bool clench::mod::isBuiltinModulesInited() {
	return _g_isBuiltinModulesInited;
}

CLCMOD_API Module *clench::mod::registerModule(
	const char *name,
	size_t nDependencies,
	const char *dependencies[],
	ModuleInit moduleInit,
	ModuleDeinit moduleDeinit,
	bool isBuiltin) {
	CLENCH_ASSERT(!isModuleRegistered(name), "The module is already registered");

	std::unique_ptr<Module> pModule = std::make_unique<Module>(
		name,
		moduleInit,
		moduleDeinit,
		isBuiltin);

	for (size_t i = 0; i < nDependencies; ++i) {
		pModule->dependencies.insert(dependencies[i]);
	}

	Module *pModuleRawPtr = pModule.get();

	g_registeredModules[name] = std::move(pModule);

	return pModuleRawPtr;
}

CLCMOD_API bool clench::mod::isModuleRegistered(const char *name) {
	return g_registeredModules.count(name);
}

CLCMOD_API void clench::mod::unregisterModule(const char *name) {
	CLENCH_ASSERT(isModuleRegistered(name), "The module has not already registered");
	g_registeredModules.erase(name);
}

struct ModuleValidityCheckContext {
	std::set<Module *> walkedModulesToLoad;
};

static bool _verifyModule(ModuleValidityCheckContext &context, Module *pModule) {
	if (context.walkedModulesToLoad.count(pModule)) {
		// Cyclic dependency detected.
		return false;
	}

	context.walkedModulesToLoad.insert(pModule);

	for (auto &i : pModule->dependencies) {
		if (auto it = g_registeredModules.find(i); it != g_registeredModules.end()) {
			ModuleValidityCheckContext newContext = context;

			if (!_verifyModule(newContext, it->second.get()))
				return false;
		} else {
			// Dependency not found.
			return false;
		}
	}

	return true;
}

bool clench::mod::verifyModule(Module *module) {
	ModuleValidityCheckContext context;

	return _verifyModule(context, module);
}

CLCMOD_API bool clench::mod::loadModule(const char *name, LoadModuleFlags flags) {
	CLENCH_ASSERT(isModuleRegistered(name), "The module has not already registered");
	Module *pModule = g_registeredModules.at(name).get();

	if (!(flags & LMOD_DEPENDENCY)) {
		CLENCH_ASSERT(!pModule->isManuallyLoaded, "The module has been manually loaded");
		pModule->isManuallyLoaded = true;
	}

#ifndef NDEBUG
	CLENCH_ASSERT(verifyModule(pModule), "Module verification failed");
#endif

	if (!pModule->isInited) {
		pModule->isInited = true;

		{
			std::unordered_set<std::string> loadedDependencies;

			for (auto &i : pModule->dependencies) {
				if (!loadModule(i.c_str(), LMOD_DEPENDENCY)) {
					for (auto &j : loadedDependencies)
						unloadModule(j.c_str(), LMOD_DEPENDENCY);
					pModule->isInited = false;
					return false;
				}
				loadedDependencies.insert(i);
			}
		}
		if (!pModule->moduleInit()) {
			for (auto &j : pModule->dependencies)
				unloadModule(j.c_str(), LMOD_DEPENDENCY);
			pModule->isInited = false;
			return false;
		}
	}

	++pModule->refCount;

	return true;
}

CLCMOD_API void clench::mod::unloadModule(const char *name, UnloadModuleFlags flags) {
	CLENCH_ASSERT(isModuleRegistered(name), "The module is not already registered");
	Module *pModule = g_registeredModules.at(name).get();

	CLENCH_ASSERT(pModule->refCount, "The module is not already loaded");
	CLENCH_ASSERT(pModule->isInited, "The module is not initialized");

	--pModule->refCount;

	if (pModule->isReleasable()) {
		pModule->moduleDeinit();
		for (auto &i : pModule->dependencies)
			unloadModule(i.c_str(), UNLMOD_DEPENDENCY);
		pModule->isInited = false;
	}
}
