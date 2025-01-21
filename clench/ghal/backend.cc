#include "backend.h"
#include <clench/utils/assert.h>
#include <clench/utils/logger.h>

using namespace clench;
using namespace clench::ghal;

CLCGHAL_API peff::HashMap<std::string_view, peff::RcObjectPtr<GHALBackend>> ghal::g_registeredGHALBackends;

CLCGHAL_API clench::ghal::GHALBackend::GHALBackend(const char *backendId, peff::Alloc *selfAllocator) : backendId(backendId), selfAllocator(selfAllocator), isInited(false) {}
CLCGHAL_API clench::ghal::GHALBackend::~GHALBackend() {
	assert(("The backend has not been deinitialized yet", !isInited));
}

CLCGHAL_API void GHALBackend::onRefZero() noexcept {
	peff::destroyAndRelease(selfAllocator.get(), this, sizeof(std::max_align_t));
}

bool GHALBackend::init() {
	if (!doInit())
		return false;
	isInited = true;
	return true;
}

bool GHALBackend::deinit() {
	if(!doDeinit())
		return false;
	isInited = false;
	return true;
}

CLCGHAL_API GHALError::GHALError(const char *msg) : runtime_error(msg) {}
CLCGHAL_API GHALError::~GHALError() {}

CLCGHAL_API void clench::ghal::registerGHALBackend(GHALBackend *backend) {
	CLENCH_DEBUG_LOG("GHAL", "Registering GHAL backend: %s", backend->backendId);
	CLENCH_ASSERT(!g_registeredGHALBackends.contains(backend->backendId), "GHAL backend with the same name already exists");

	g_registeredGHALBackends.insert(backend->backendId, backend);
}

CLCGHAL_API void clench::ghal::unregisterGHALBackend(const char *id) {
	CLENCH_DEBUG_LOG("GHAL", "Unregistering GHAL backend: %s", id);

	auto it = g_registeredGHALBackends.find(id);

	if (it == g_registeredGHALBackends.end()) {
		assert(("GHAL backend not found", false));
	}
	assert(("The GHAL backend has not deinitialized yet", it.value()->isInited));

	g_registeredGHALBackends.remove(id);
}

CLCGHAL_API GHALBackend *clench::ghal::getGHALBackend(const char *id) {
	CLENCH_DEBUG_LOG("GHAL", "Querying GHAL backend: %s", id);

	auto it = g_registeredGHALBackends.find(id);

	if (it == g_registeredGHALBackends.end()) {
		CLENCH_DEBUG_LOG("GHAL", "Querying GHAL backend: %s - not found", id);
		return nullptr;
	}

	CLENCH_DEBUG_LOG("GHAL", "Querying GHAL backend: %s - found", id);
	return it.value().get();
}

CLCGHAL_API std::optional<std::pair<bool, const char *>> clench::ghal::scanAndInitRegisteredGHALBackends() {
	CLENCH_DEBUG_LOG("GHAL", "Rescanning and initializing new GHAL backends...");
	for (auto i = g_registeredGHALBackends.begin(); i != g_registeredGHALBackends.end(); ++i) {
		if (!i.value()->isInited) {
			bool initResult = initRegisteredGHALBackend(i.key().data());
			if (!initResult) {
				return std::pair<bool, const char *>(false, i.key().data());
			}
		}
	}

	return {};
}

CLCGHAL_API std::optional<std::pair<bool, const char *>> clench::ghal::deinitInitedRegisteredGHALBackends() {
	CLENCH_DEBUG_LOG("GHAL", "Deinitializing GHAL backends...");
	for (auto i = g_registeredGHALBackends.begin(); i != g_registeredGHALBackends.end(); ++i) {
		if (i.value()->isInited) {
			bool initResult = deinitRegisteredGHALBackend(i.key().data());
			if (!initResult) {
				return std::pair<bool, const char *>(false, i.key().data());
			}
		}
	}

	return {};
}

CLCGHAL_API bool clench::ghal::initRegisteredGHALBackend(const char *id) {
	CLENCH_DEBUG_LOG("GHAL", "Initializing GHAL backend: %s", id);
	if (auto it = g_registeredGHALBackends.find(id); it != g_registeredGHALBackends.end()) {
		bool result = it.value()->init();
		if (result) {
			CLENCH_DEBUG_LOG("GHAL", "Initialized GHAL backend: %s", id);
		} else {
			CLENCH_DEBUG_LOG("GHAL", "Error initializing GHAL backend: %s", id);
		}
		return result;
	}
	CLENCH_DEBUG_LOG("GHAL", "Initializing GHAL backend: %s - not found", id);
	return false;
}

CLCGHAL_API bool clench::ghal::deinitRegisteredGHALBackend(const char *id) {
	CLENCH_DEBUG_LOG("GHAL", "Deinitializing GHAL backend: %s", id);
	if (auto it = g_registeredGHALBackends.find(id); it != g_registeredGHALBackends.end()) {
		if (!it.value()->isInited)
			return true;
		bool result = it.value()->deinit();
		if (result) {
			CLENCH_DEBUG_LOG("GHAL", "Deinitialized GHAL backend: %s", id);
		} else {
			CLENCH_DEBUG_LOG("GHAL", "Error deinitializing GHAL backend: %s", id);
		}
		return result;
	}
	CLENCH_DEBUG_LOG("GHAL", "Deinitializing GHAL backend: %s - not found", id);
	return false;
}

CLCGHAL_API GHALDevice *clench::ghal::createGHALDevice(const peff::List<std::string_view> &preferredBackendNames) {
	peff::DynArray<GHALBackend *> deviceCreationQueue;
	if (!deviceCreationQueue.resize(g_registeredGHALBackends.size()))
		return nullptr;
	if (preferredBackendNames.size()) {
		size_t deviceCreationQueueBackIndex = 0;
		peff::Set<GHALBackend *> pushedBackends;

		for (auto i = preferredBackendNames.beginConst(); i != preferredBackendNames.endConst(); ++i) {
			if (auto it = clench::ghal::g_registeredGHALBackends.find(*i);
				it != clench::ghal::g_registeredGHALBackends.end()) {
				GHALBackend *curBackend = it.value().get();
				deviceCreationQueue.at(deviceCreationQueueBackIndex++) = curBackend;
				if (!pushedBackends.insert(std::move(curBackend)))
					return nullptr;
			}
		}

		for (auto i = g_registeredGHALBackends.begin(); i != g_registeredGHALBackends.end(); ++i) {
			if (!pushedBackends.contains(i.value().get())) {
				deviceCreationQueue.at(deviceCreationQueueBackIndex++) = i.value().get();
			}
		}
	} else {
		size_t deviceCreationQueueBackIndex = 0;
		for (auto i = g_registeredGHALBackends.begin(); i != g_registeredGHALBackends.end(); ++i) {
			deviceCreationQueue.at(deviceCreationQueueBackIndex++) = i.value().get();
		}
	}

	for (size_t i = 0; i < deviceCreationQueue.size(); ++i) {
		auto curBackend = deviceCreationQueue.at(i);
		if (!curBackend->isInited) {
			CLENCH_DEBUG_LOG("GHAL", "Creating GHAL device using GHAL backend: %s - skipped due to uninitialized", curBackend->backendId);
			continue;
		}
		CLENCH_DEBUG_LOG("GHAL", "Creating GHAL device using GHAL backend: %s", curBackend->backendId);
		if (auto device = curBackend->createDevice(); device) {
			CLENCH_DEBUG_LOG("GHAL", "Created GHAL device using GHAL backend: %s", curBackend->backendId);
			return device;
		}
	}

	return nullptr;
}
