#ifndef _CLENCH_UTILS_DYNLIB_HH_
#define _CLENCH_UTILS_DYNLIB_HH_

#include <stdexcept>
#include <string>

#ifdef _WIN32
	#include <Windows.h>
#endif

#if defined(DYNLIBPROC)
	#error Macros conflicted
#endif

#if defined(_MSC_VER)
	#if _DLL
		#define DYNLIBPROC __declspec(dllexport)
	#else
		#define DYNLIBPROC __declspec(dllimport)
	#endif
#elif defined(__GNUC__)
	#define DYNLIBPROC __attribute__(( __visibility__("default")))
#else
	#error Not implemented yet
#endif

namespace clench {
	namespace utils {
		class DynLib final {
		protected:
#ifdef _WIN32
			HMODULE _hModule;
#endif

		public:
			DynLib(std::string path) {
#if defined(_WIN32)
				if (!(_hModule = LoadLibraryA(path.c_str())))
					throw std::runtime_error("Error loading library with HRESULT " + std::to_string(GetLastError()));
#else
	#error Not implemented yet
#endif
			}

			DynLib(std::wstring path) {
#if defined(_WIN32)
				if (!(_hModule = LoadLibraryW(path.c_str())))
					throw std::runtime_error("Error loading library with HRESULT " + std::to_string(GetLastError()));
#else
	#error Not implemented yet
#endif
			}

			~DynLib() {
#if defined(_WIN32)
				if (_hModule)
					FreeLibrary(_hModule);
#else
	#error Not implemented yet
#endif
			}

			template <typename T>
			T getSymbol(std::string name) {
#ifdef _WIN32
				return (T)GetProcAddress(_hModule, name.c_str());
#else
	#error Not implemented yet
#endif
			}
		};
	}
}

#endif
