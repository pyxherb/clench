#include "backend.h"
#include <clench/utils/assert.h>

using namespace clench;
using namespace clench::rhi;

CLCRHI_API std::unordered_map<std::string, std::unique_ptr<RHIBackend>> clench::rhi::g_registeredRHIBackends;

CLCRHI_API clench::rhi::RHIBackend::RHIBackend(const char *backendId) : backendId(backendId) {}
CLCRHI_API clench::rhi::RHIBackend::~RHIBackend() {}

CLCRHI_API void clench::rhi::registerRHIBackend(RHIBackend *backend) {
	std::string id = backend->backendId;

	CLENCH_ASSERT(!g_registeredRHIBackends.count(id), "RHI backend with the same name already exists");

	g_registeredRHIBackends[id] = std::unique_ptr<RHIBackend>(backend);
}

CLCRHI_API void clench::rhi::unregisterRHIBackend(const std::string &id) {
	auto it = g_registeredRHIBackends.find(id);

	CLENCH_ASSERT(it != g_registeredRHIBackends.end(), "No such RHI Backend");

	g_registeredRHIBackends.erase(it);
}

CLCRHI_API RHIBackend *clench::rhi::getRHIBackend(const std::string &id) {
	auto it = g_registeredRHIBackends.find(id);

	if (it == g_registeredRHIBackends.end())
		return nullptr;

	return it->second.get();
}

CLCRHI_API RHIDevice *clench::rhi::createRHIDevice() {
	for (const auto &i : g_registeredRHIBackends)
		if (auto device = i.second->createDevice(); device)
			return device;

	return nullptr;
}