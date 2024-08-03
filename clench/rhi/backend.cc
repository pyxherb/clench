#include "backend.h"
#include <clench/utils/assert.h>
#include <clench/utils/logger.h>

using namespace clench;
using namespace clench::rhi;

CLCRHI_API std::unordered_map<std::string, std::unique_ptr<RHIBackend>> clench::rhi::g_registeredRHIBackends;

CLCRHI_API clench::rhi::RHIBackend::RHIBackend(const char *backendId) : backendId(backendId) {}
CLCRHI_API clench::rhi::RHIBackend::~RHIBackend() {}

CLCRHI_API RHIError::RHIError(const char *msg) : runtime_error(msg) {}
CLCRHI_API RHIError::~RHIError() {}

CLCRHI_API void clench::rhi::registerRHIBackend(RHIBackend *backend) {
	std::string id = backend->backendId;

	CLENCH_DEBUG_LOG("Registering RHI backend: %s\n", id.c_str());
	CLENCH_ASSERT(!g_registeredRHIBackends.count(id), "RHI backend with the same name already exists");

	g_registeredRHIBackends[id] = std::unique_ptr<RHIBackend>(backend);
}

CLCRHI_API void clench::rhi::unregisterRHIBackend(const char *id) {
	CLENCH_DEBUG_LOG("Unregistering RHI backend: %s\n", id);

	auto it = g_registeredRHIBackends.find(id);

	CLENCH_ASSERT(it != g_registeredRHIBackends.end(), "No such RHI Backend");

	g_registeredRHIBackends.erase(it);
}

CLCRHI_API RHIBackend *clench::rhi::getRHIBackend(const char *id) {
	CLENCH_DEBUG_LOG("Querying RHI backend: %s\n", id);

	auto it = g_registeredRHIBackends.find(id);

	if (it == g_registeredRHIBackends.end()) {
		CLENCH_DEBUG_LOG("Querying RHI backend: %s - not found\n", id);
		return nullptr;
	}

	CLENCH_DEBUG_LOG("Querying RHI backend: %s - found\n", id);
	return it->second.get();
}

CLCRHI_API RHIDevice *clench::rhi::createRHIDevice() {
	for (const auto &i : g_registeredRHIBackends) {
		CLENCH_DEBUG_LOG("Creating RHI device using RHI backend: %s\n", i.first.c_str());
		if (auto device = i.second->createDevice(); device) {
			CLENCH_DEBUG_LOG("Created RHI device using RHI backend: %s\n", i.first.c_str());
			return device;
		}
	}

	return nullptr;
}
