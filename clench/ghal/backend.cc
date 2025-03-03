#include "backend.h"
#include <clench/utils/assert.h>
#include <clench/utils/logger.h>

using namespace clench;
using namespace clench::ghal;

CLCGHAL_API peff::HashMap<std::string_view, peff::RcObjectPtr<Backend>> ghal::g_registeredBackends;

CLCGHAL_API clench::ghal::Backend::Backend(const char *backendId, peff::Alloc *selfAllocator) : backendId(backendId), selfAllocator(selfAllocator), isInited(false) {}
CLCGHAL_API clench::ghal::Backend::~Backend() {
	assert(("The backend has not been deinitialized yet", !isInited));
}

CLCGHAL_API void Backend::onRefZero() noexcept {
	dealloc();
}

bool Backend::init() {
	if (!doInit())
		return false;
	isInited = true;
	return true;
}

bool Backend::deinit() {
	if (!doDeinit())
		return false;
	isInited = false;
	return true;
}

CLCGHAL_API GHALError::GHALError(const char *msg) : runtime_error(msg) {}
CLCGHAL_API GHALError::~GHALError() {}

CLCGHAL_API void clench::ghal::registerBackend(Backend *backend) {
	CLENCH_DEBUG_LOG("GHAL", "Registering GHAL backend: %s", backend->backendId);
	CLENCH_ASSERT(!g_registeredBackends.contains(backend->backendId), "GHAL backend with the same name already exists");

	g_registeredBackends.insert(backend->backendId, backend);
}

CLCGHAL_API void clench::ghal::unregisterBackend(const char *id) {
	CLENCH_DEBUG_LOG("GHAL", "Unregistering GHAL backend: %s", id);

	auto it = g_registeredBackends.find(id);

	if (it == g_registeredBackends.end()) {
		assert(("GHAL backend not found", false));
	}
	assert(("The GHAL backend has not deinitialized yet", it.value()->isInited));

	g_registeredBackends.remove(id);
}

CLCGHAL_API Backend *clench::ghal::getBackend(const char *id) {
	CLENCH_DEBUG_LOG("GHAL", "Querying GHAL backend: %s", id);

	auto it = g_registeredBackends.find(id);

	if (it == g_registeredBackends.end()) {
		CLENCH_DEBUG_LOG("GHAL", "Querying GHAL backend: %s - not found", id);
		return nullptr;
	}

	CLENCH_DEBUG_LOG("GHAL", "Querying GHAL backend: %s - found", id);
	return it.value().get();
}

CLCGHAL_API std::optional<std::pair<bool, const char *>> clench::ghal::scanAndInitRegisteredBackends() {
	CLENCH_DEBUG_LOG("GHAL", "Rescanning and initializing new GHAL backends...");
	for (auto i = g_registeredBackends.begin(); i != g_registeredBackends.end(); ++i) {
		if (!i.value()->isInited) {
			bool initResult = initRegisteredBackend(i.key().data());
			if (!initResult) {
				return std::pair<bool, const char *>(false, i.key().data());
			}
		}
	}

	return {};
}

CLCGHAL_API std::optional<std::pair<bool, const char *>> clench::ghal::deinitInitedRegisteredBackends() {
	CLENCH_DEBUG_LOG("GHAL", "Deinitializing GHAL backends...");
	for (auto i = g_registeredBackends.begin(); i != g_registeredBackends.end(); ++i) {
		if (i.value()->isInited) {
			bool initResult = deinitRegisteredBackend(i.key().data());
			if (!initResult) {
				return std::pair<bool, const char *>(false, i.key().data());
			}
		}
	}

	return {};
}

CLCGHAL_API bool clench::ghal::initRegisteredBackend(const char *id) {
	CLENCH_DEBUG_LOG("GHAL", "Initializing GHAL backend: %s", id);
	if (auto it = g_registeredBackends.find(id); it != g_registeredBackends.end()) {
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

CLCGHAL_API bool clench::ghal::deinitRegisteredBackend(const char *id) {
	CLENCH_DEBUG_LOG("GHAL", "Deinitializing GHAL backend: %s", id);
	if (auto it = g_registeredBackends.find(id); it != g_registeredBackends.end()) {
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

CLCGHAL_API base::ExceptionPtr clench::ghal::createDevice(Device *&ghalDeviceOut, const peff::List<std::string_view> &preferredBackendNames) {
	peff::DynArray<Backend *> deviceCreationQueue;
	if (!deviceCreationQueue.resize(g_registeredBackends.size()))
		return nullptr;
	if (preferredBackendNames.size()) {
		size_t deviceCreationQueueBackIndex = 0;
		peff::Set<Backend *> pushedBackends;

		for (auto i = preferredBackendNames.beginConst(); i != preferredBackendNames.endConst(); ++i) {
			if (auto it = clench::ghal::g_registeredBackends.find(*i);
				it != clench::ghal::g_registeredBackends.end()) {
				Backend *curBackend = it.value().get();
				deviceCreationQueue.at(deviceCreationQueueBackIndex++) = curBackend;
				if (!pushedBackends.insert(std::move(curBackend)))
					return nullptr;
			}
		}

		for (auto i = g_registeredBackends.begin(); i != g_registeredBackends.end(); ++i) {
			if (!pushedBackends.contains(i.value().get())) {
				deviceCreationQueue.at(deviceCreationQueueBackIndex++) = i.value().get();
			}
		}
	} else {
		size_t deviceCreationQueueBackIndex = 0;
		for (auto i = g_registeredBackends.begin(); i != g_registeredBackends.end(); ++i) {
			deviceCreationQueue.at(deviceCreationQueueBackIndex++) = i.value().get();
		}
	}

	Device *device;
	for (size_t i = 0; i < deviceCreationQueue.size(); ++i) {
		auto curBackend = deviceCreationQueue.at(i);
		if (!curBackend->isInited) {
			CLENCH_DEBUG_LOG("GHAL", "Creating GHAL device using GHAL backend: %s - skipped due to uninitialized", curBackend->backendId);
			continue;
		}
		CLENCH_DEBUG_LOG("GHAL", "Creating GHAL device using GHAL backend: %s", curBackend->backendId);
		base::ExceptionPtr e = curBackend->createDevice(device);
		if (e) {
			if (e->typeUUID != EXCEPTION_TYPE_GHAL) {
				return e;
			}
			CLENCH_DEBUG_LOG("WSAL", "Error creating window using GHAL backend `%s`: %s", curBackend->backendId, e->what());
			e.reset();
		} else {
			CLENCH_DEBUG_LOG("GHAL", "Created GHAL device using GHAL backend: %s", curBackend->backendId);
			ghalDeviceOut = device;
			return {};
		}
	}

	return nullptr;
}
