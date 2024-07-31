#ifndef _CLENCH_RHI_BACKEND_H_
#define _CLENCH_RHI_BACKEND_H_

#include "basedefs.h"
#include <string>
#include <unordered_map>
#include <memory>

namespace clench {
	namespace rhi {
		class RHIDevice;

		class RHIBackend {
		public:
			const std::string backendId;

			NO_COPY_MOVE_METHODS(RHIBackend);

			CLCRHI_API RHIBackend(const char* backendId);
			CLCRHI_API virtual ~RHIBackend();

			virtual RHIDevice* createDevice() = 0;
		};

		CLCRHI_API extern std::unordered_map<std::string, std::unique_ptr<RHIBackend>> g_registeredRHIBackends;

		CLCRHI_API void registerRHIBackend(RHIBackend* backend);
		CLCRHI_API void unregisterRHIBackend(const std::string& id);
		CLCRHI_API RHIBackend* getRHIBackend(const std::string& id);

		CLCRHI_API void registerBuiltinRHIBackends();

		CLCRHI_API RHIDevice* createRHIDevice();
	}
}

#endif