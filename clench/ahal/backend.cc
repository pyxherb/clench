#include "backend.h"
#include <clench/utils/assert.h>
#include <clench/utils/logger.h>

using namespace clench;
using namespace clench::ahal;

CLCAHAL_API peff::HashMap<std::string_view, peff::RcObjectPtr<Backend>> ahal::g_registeredBackends;

CLCAHAL_API clench::ahal::Backend::Backend(const char *backendId, peff::Alloc *selfAllocator) : backendId(backendId), selfAllocator(selfAllocator), isInited(false) {}
CLCAHAL_API clench::ahal::Backend::~Backend() {
	assert(("The backend has not been deinitialized yet", !isInited));
}

CLCAHAL_API void Backend::onRefZero() noexcept {
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

CLCAHAL_API AHALError::AHALError(const char *msg) : runtime_error(msg) {}
CLCAHAL_API AHALError::~AHALError() {}

CLCAHAL_API void clench::ahal::registerBackend(Backend *backend) {
	CLENCH_DEBUG_LOG("AHAL", "Registering AHAL backend: %s", backend->backendId);
	CLENCH_ASSERT(!g_registeredBackends.contains(backend->backendId), "AHAL backend with the same name already exists");

	g_registeredBackends.insert(backend->backendId, backend);
}

CLCAHAL_API void clench::ahal::unregisterBackend(const char *id) {
	CLENCH_DEBUG_LOG("AHAL", "Unregistering AHAL backend: %s", id);

	auto it = g_registeredBackends.find(id);

	if (it == g_registeredBackends.end()) {
		assert(("AHAL backend not found", false));
	}
	assert(("The AHAL backend has not deinitialized yet", it.value()->isInited));

	g_registeredBackends.remove(id);
}

CLCAHAL_API Backend *clench::ahal::getBackend(const char *id) {
	CLENCH_DEBUG_LOG("AHAL", "Querying AHAL backend: %s", id);

	auto it = g_registeredBackends.find(id);

	if (it == g_registeredBackends.end()) {
		CLENCH_DEBUG_LOG("AHAL", "Querying AHAL backend: %s - not found", id);
		return nullptr;
	}

	CLENCH_DEBUG_LOG("AHAL", "Querying AHAL backend: %s - found", id);
	return it.value().get();
}

CLCAHAL_API std::optional<std::pair<bool, const char *>> clench::ahal::scanAndInitRegisteredBackends() {
	CLENCH_DEBUG_LOG("AHAL", "Rescanning and initializing new AHAL backends...");
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

CLCAHAL_API std::optional<std::pair<bool, const char *>> clench::ahal::deinitInitedRegisteredBackends() {
	CLENCH_DEBUG_LOG("AHAL", "Deinitializing AHAL backends...");
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

CLCAHAL_API bool clench::ahal::initRegisteredBackend(const char *id) {
	CLENCH_DEBUG_LOG("AHAL", "Initializing AHAL backend: %s", id);
	if (auto it = g_registeredBackends.find(id); it != g_registeredBackends.end()) {
		bool result = it.value()->init();
		if (result) {
			CLENCH_DEBUG_LOG("AHAL", "Initialized AHAL backend: %s", id);
		} else {
			CLENCH_DEBUG_LOG("AHAL", "Error initializing AHAL backend: %s", id);
		}
		return result;
	}
	CLENCH_DEBUG_LOG("AHAL", "Initializing AHAL backend: %s - not found", id);
	return false;
}

CLCAHAL_API bool clench::ahal::deinitRegisteredBackend(const char *id) {
	CLENCH_DEBUG_LOG("AHAL", "Deinitializing AHAL backend: %s", id);
	if (auto it = g_registeredBackends.find(id); it != g_registeredBackends.end()) {
		if (!it.value()->isInited)
			return true;
		bool result = it.value()->deinit();
		if (result) {
			CLENCH_DEBUG_LOG("AHAL", "Deinitialized AHAL backend: %s", id);
		} else {
			CLENCH_DEBUG_LOG("AHAL", "Error deinitializing AHAL backend: %s", id);
		}
		return result;
	}
	CLENCH_DEBUG_LOG("AHAL", "Deinitializing AHAL backend: %s - not found", id);
	return false;
}

CLCAHAL_API base::ExceptionPtr clench::ahal::createDevice(Device *&ahalDeviceOut, const peff::List<std::string_view> &preferredBackendNames) {
	peff::DynArray<Backend *> deviceCreationQueue;
	if (!deviceCreationQueue.resize(g_registeredBackends.size()))
		return nullptr;
	if (preferredBackendNames.size()) {
		size_t deviceCreationQueueBackIndex = 0;
		peff::Set<Backend *> pushedBackends;

		for (auto i = preferredBackendNames.beginConst(); i != preferredBackendNames.endConst(); ++i) {
			if (auto it = clench::ahal::g_registeredBackends.find(*i);
				it != clench::ahal::g_registeredBackends.end()) {
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
			CLENCH_DEBUG_LOG("AHAL", "Creating AHAL device using AHAL backend: %s - skipped due to uninitialized", curBackend->backendId);
			continue;
		}
		CLENCH_DEBUG_LOG("AHAL", "Creating AHAL device using AHAL backend: %s", curBackend->backendId);
		base::ExceptionPtr e = curBackend->createDevice(peff::getDefaultAlloc(), device);
		if (e) {
			if (e->typeUUID != EXCEPTION_TYPE_AHAL) {
				return e;
			}
			CLENCH_DEBUG_LOG("WSAL", "Error creating window using AHAL backend `%s`: %s", curBackend->backendId, e->what());
			e.reset();
		} else {
			CLENCH_DEBUG_LOG("AHAL", "Created AHAL device using AHAL backend: %s", curBackend->backendId);
			ahalDeviceOut = device;
			return {};
		}
	}

	return nullptr;
}
