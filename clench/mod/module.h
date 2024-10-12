#ifndef _CLENCH_BASE_MODULE_H_
#define _CLENCH_BASE_MODULE_H_

#include "basedefs.h"
#include <clench/utils/assert.h>
#include <clench/utils/logger.h>
#include <unordered_map>
#include <string>
#include <memory>
#include <atomic>
#include <unordered_set>
#include <stdexcept>
#include <set>

#if _WIN32
	#include <Windows.h>
#elif __unix__
	#include <dlfcn.h>
#endif

namespace clench {
	namespace mod {
		typedef bool (*ModuleInit)();
		typedef void (*ModuleDeinit)();

		struct BuiltinModuleRegistry {
			BuiltinModuleRegistry *next;
			const char *name;
			size_t nDependencies;
			const char **dependencies;
			ModuleInit moduleInit;
			ModuleDeinit moduleDeinit;
			bool autoLoad;
			size_t moduleRegisterCounter;

#ifdef _WIN32
			HMODULE nativeHandle;
#endif
		};

		CLCMOD_API extern BuiltinModuleRegistry *g_builtinModuleRegistries;
		CLCMOD_API extern bool g_isBuiltinModulesInited;

		class Module final {
		public:
			std::atomic_uint32_t refCount = 0;

			std::string name;
			ModuleInit moduleInit;
			ModuleDeinit moduleDeinit;
			bool isBuiltin;
			bool isInited = false;
			bool isManuallyLoaded = false;
			bool isExternal = false;
			std::unordered_set<std::string> dependencies;

			size_t moduleRegisterCounterValue = SIZE_MAX;

#ifdef _WIN32
			HMODULE nativeHandle = NULL;
#elif __unix__
			void *nativeHandle = nullptr;
#endif

			CLCMOD_API Module(
				const char *name,
				ModuleInit moduleInit,
				ModuleDeinit moduleDeinit,
				bool isBuiltin);
			CLCMOD_API ~Module();

			FORCEINLINE bool isReleasable() {
				return !refCount;
			}
		};

		CLCMOD_API extern std::unordered_map<std::string, std::unique_ptr<Module>> g_registeredModules;
		CLCMOD_API extern size_t g_moduleRegisterCounter;

		CLCMOD_API bool initBuiltinModules();
		CLCMOD_API void deinitModules();
		CLCMOD_API bool isBuiltinModulesInited();

		CLCMOD_API Module *registerModule(
			const char *name,
			size_t nDependencies,
			const char *dependencies[],
			ModuleInit moduleInit,
			ModuleDeinit moduleDeinit,
			bool isBuiltin = false);
		CLCMOD_API bool isModuleRegistered(const char *name);
		CLCMOD_API void unregisterModule(const char *name);

		bool verifyModule(Module *module);

		using LoadModuleFlags = uint8_t;
		constexpr static LoadModuleFlags
			LMOD_DEPENDENCY = 0x01;
		CLCMOD_API bool loadModule(const char *name, LoadModuleFlags flags = 0);

		using UnloadModuleFlags = uint8_t;
		constexpr static UnloadModuleFlags
			UNLMOD_DEPENDENCY = 0x01;
		CLCMOD_API void unloadModule(const char *name, UnloadModuleFlags flags = 0);

		CLCMOD_API Module *registerExternalModule(const char *name);
	}
}

#endif
