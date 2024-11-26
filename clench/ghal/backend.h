#ifndef _CLENCH_GHAL_BACKEND_H_
#define _CLENCH_GHAL_BACKEND_H_

#include "basedefs.h"
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <set>
#include <memory>
#include <list>
#include <vector>

namespace clench {
	namespace ghal {
		class GHALDevice;

		class GHALBackend {
		public:
			const std::string backendId;

			NO_COPY_MOVE_METHODS(GHALBackend);

			CLCGHAL_API GHALBackend(const char* backendId);
			CLCGHAL_API virtual ~GHALBackend();

			virtual GHALDevice* createDevice() = 0;
		};

		class GHALError : public std::runtime_error {
		public:
			CLCGHAL_API GHALError(const char *msg);
			CLCGHAL_API virtual ~GHALError();
		};

		CLCGHAL_API extern std::unordered_map<std::string, std::unique_ptr<GHALBackend>> g_registeredGHALBackends;

		CLCGHAL_API void registerGHALBackend(GHALBackend* backend);
		CLCGHAL_API void unregisterGHALBackend(const char* id);
		CLCGHAL_API GHALBackend* getGHALBackend(const char* id);

		CLCGHAL_API void registerBuiltinGHALBackends();

		CLCGHAL_API GHALDevice *createGHALDevice(const std::list<std::string> &preferredBackendNames = {});
	}
}

#endif
