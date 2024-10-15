#include "backend.h"
#include <clench/utils/assert.h>
#include <clench/utils/logger.h>

using namespace clench;
using namespace clench::ghal;

CLCGHAL_API std::unordered_map<std::string, std::unique_ptr<GHALBackend>> clench::ghal::g_registeredGHALBackends;

CLCGHAL_API clench::ghal::GHALBackend::GHALBackend(const char *backendId) : backendId(backendId) {}
CLCGHAL_API clench::ghal::GHALBackend::~GHALBackend() {}

CLCGHAL_API GHALError::GHALError(const char *msg) : runtime_error(msg) {}
CLCGHAL_API GHALError::~GHALError() {}

CLCGHAL_API void clench::ghal::registerGHALBackend(GHALBackend *backend) {
	std::string id = backend->backendId;

	CLENCH_DEBUG_LOG("GHAL", "Registering GHAL backend: %s", id.c_str());
	CLENCH_ASSERT(!g_registeredGHALBackends.count(id), "GHAL backend with the same name already exists");

	g_registeredGHALBackends[id] = std::unique_ptr<GHALBackend>(backend);
}

CLCGHAL_API void clench::ghal::unregisterGHALBackend(const char *id) {
	CLENCH_DEBUG_LOG("GHAL", "Unregistering GHAL backend: %s", id);

	auto it = g_registeredGHALBackends.find(id);

	CLENCH_ASSERT(it != g_registeredGHALBackends.end(), "No such GHAL Backend");

	g_registeredGHALBackends.erase(it);
}

CLCGHAL_API GHALBackend *clench::ghal::getGHALBackend(const char *id) {
	CLENCH_DEBUG_LOG("GHAL", "Querying GHAL backend: %s", id);

	auto it = g_registeredGHALBackends.find(id);

	if (it == g_registeredGHALBackends.end()) {
		CLENCH_DEBUG_LOG("GHAL", "Querying GHAL backend: %s - not found", id);
		return nullptr;
	}

	CLENCH_DEBUG_LOG("GHAL", "Querying GHAL backend: %s - found", id);
	return it->second.get();
}

CLCGHAL_API GHALDevice *clench::ghal::createGHALDevice(const std::list<std::string> &preferredBackendNames) {
	std::vector<GHALBackend *> deviceCreationQueue;
	deviceCreationQueue.resize(g_registeredGHALBackends.size());
	if (preferredBackendNames.size()) {
		size_t deviceCreationQueueBackIndex = 0;
		std::set<GHALBackend *> pushedBackends;

		for (auto &i : preferredBackendNames) {
			if (auto it = clench::ghal::g_registeredGHALBackends.find(i);
				it != clench::ghal::g_registeredGHALBackends.end()) {
				GHALBackend *curBackend = it->second.get();
				deviceCreationQueue[deviceCreationQueueBackIndex++] = curBackend;
				pushedBackends.insert(curBackend);
			}
		}

		for (auto &i : g_registeredGHALBackends) {
			if (!pushedBackends.count(i.second.get())) {
				deviceCreationQueue[deviceCreationQueueBackIndex++] = i.second.get();
			}
		}
	} else {
		size_t deviceCreationQueueBackIndex = 0;
		for (const auto &i : g_registeredGHALBackends) {
			deviceCreationQueue[deviceCreationQueueBackIndex++] = i.second.get();
		}
	}

	for (auto i : deviceCreationQueue) {
		CLENCH_DEBUG_LOG("GHAL", "Creating GHAL device using GHAL backend: %s", i->backendId.c_str());
		if (auto device = i->createDevice(); device) {
			CLENCH_DEBUG_LOG("GHAL", "Created GHAL device using GHAL backend: %s", i->backendId.c_str());
			return device;
		}
	}

	return nullptr;
}
