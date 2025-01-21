#include "backend.h"
#include <clench/utils/logger.h>
#include <clench/utils/assert.h>

using namespace clench;
using namespace clench::wsal;

CLCWSAL_API peff::HashMap<std::string_view, peff::RcObjectPtr<Backend>> clench::wsal::g_registeredWSALBackends;

CLCWSAL_API Backend::Backend(const char *backendId, peff::Alloc *selfAllocator, peff::Alloc *resourceAllocator)
	: backendId(backendId),
	  selfAllocator(selfAllocator),
	  resourceAllocator(resourceAllocator) {
}

CLCWSAL_API Backend::~Backend(){
	assert(("The backend has not been deinitialized yet", !isInited));
}

CLCWSAL_API void Backend::onRefZero() noexcept {
	peff::destroyAndRelease<Backend>(selfAllocator.get(), this, sizeof(std::max_align_t));
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

CLCWSAL_API void clench::wsal::registerWSALBackend(Backend *backend) {
	CLENCH_DEBUG_LOG("WSAL", "Registering WSAL backend: %s", backend->backendId);
	CLENCH_ASSERT(!g_registeredWSALBackends.contains(backend->backendId), "WSAL backend with the same name already exists");

	g_registeredWSALBackends.insert(backend->backendId, backend);
}

CLCWSAL_API void clench::wsal::unregisterWSALBackend(const char *id) {
	CLENCH_DEBUG_LOG("WSAL", "Unregistering WSAL backend: %s", id);

	auto it = g_registeredWSALBackends.find(id);

	if (it == g_registeredWSALBackends.end()) {
		assert(("WSAL backend not found", false));
	}
	assert(("The WSAL backend has not deinitialized yet", it.value()->isInited));

	g_registeredWSALBackends.remove(id);
}

CLCWSAL_API Backend *clench::wsal::getWSALBackend(const char *id) {
	CLENCH_DEBUG_LOG("WSAL", "Querying WSAL backend: %s", id);

	auto it = g_registeredWSALBackends.find(id);

	if (it == g_registeredWSALBackends.end()) {
		CLENCH_DEBUG_LOG("WSAL", "Querying WSAL backend: %s - not found", id);
		return nullptr;
	}

	CLENCH_DEBUG_LOG("WSAL", "Querying WSAL backend: %s - found", id);
	return it.value().get();
}

CLCWSAL_API std::optional<std::pair<bool, const char *>> clench::wsal::scanAndInitRegisteredWSALBackends() {
	CLENCH_DEBUG_LOG("WSAL", "Rescanning and initializing new WSAL backends...");
	for (auto i = g_registeredWSALBackends.begin(); i != g_registeredWSALBackends.end(); ++i) {
		if (!i.value()->isInited) {
			bool initResult = initRegisteredWSALBackend(i.key().data());
			if (!initResult) {
				return std::pair<bool, const char *>(false, i.key().data());
			}
		}
	}

	return {};
}

CLCWSAL_API std::optional<std::pair<bool, const char *>> clench::wsal::deinitInitedRegisteredWSALBackends() {
	CLENCH_DEBUG_LOG("WSAL", "Deinitializing WSAL backends...");
	for (auto i = g_registeredWSALBackends.begin(); i != g_registeredWSALBackends.end(); ++i) {
		if (i.value()->isInited) {
			bool initResult = deinitRegisteredWSALBackend(i.key().data());
			if (!initResult) {
				return std::pair<bool, const char *>(false, i.key().data());
			}
		}
	}

	return {};
}

CLCWSAL_API bool clench::wsal::initRegisteredWSALBackend(const char *id) {
	CLENCH_DEBUG_LOG("WSAL", "Initializing WSAL backend: %s", id);
	if (auto it = g_registeredWSALBackends.find(id); it != g_registeredWSALBackends.end()) {
		bool result = it.value()->init();
		if (result) {
			CLENCH_DEBUG_LOG("WSAL", "Initialized WSAL backend: %s", id);
		} else {
			CLENCH_DEBUG_LOG("WSAL", "Error initializing WSAL backend: %s", id);
		}
		return result;
	}
	CLENCH_DEBUG_LOG("WSAL", "Initializing WSAL backend: %s - not found", id);
	return false;
}

CLCWSAL_API bool clench::wsal::deinitRegisteredWSALBackend(const char *id) {
	CLENCH_DEBUG_LOG("WSAL", "Deinitializing WSAL backend: %s", id);
	if (auto it = g_registeredWSALBackends.find(id); it != g_registeredWSALBackends.end()) {
		if (!it.value()->isInited)
			return true;
		bool result = it.value()->deinit();
		if (result) {
			CLENCH_DEBUG_LOG("WSAL", "Deinitialized WSAL backend: %s", id);
		} else {
			CLENCH_DEBUG_LOG("WSAL", "Error deinitializing WSAL backend: %s", id);
		}
		return result;
	}
	CLENCH_DEBUG_LOG("WSAL", "Deinitializing WSAL backend: %s - not found", id);
	return false;
}

CLCWSAL_API Window *clench::wsal::createWindow(
	CreateWindowFlags flags,
	Window *parent,
	int x,
	int y,
	int width,
	int height,
	const peff::List<std::string_view> &preferredBackendNames) {
	if (parent) {
		// TODO: Virtual windows do not have backend, add something for them.
		return parent->backend->createWindow(
			flags,
			parent,
			x,
			y,
			width,
			height
		);
	} else {
		peff::DynArray<Backend *> deviceCreationQueue;
		if (!deviceCreationQueue.resize(g_registeredWSALBackends.size()))
			return nullptr;
		if (preferredBackendNames.size()) {
			size_t deviceCreationQueueBackIndex = 0;
			peff::Set<Backend *> pushedBackends;

			for (auto i = preferredBackendNames.beginConst(); i != preferredBackendNames.endConst(); ++i) {
				if (auto it = clench::wsal::g_registeredWSALBackends.find(*i);
					it != clench::wsal::g_registeredWSALBackends.end()) {
					Backend *curBackend = it.value().get();
					deviceCreationQueue.at(deviceCreationQueueBackIndex++) = curBackend;
					if (!pushedBackends.insert(std::move(curBackend)))
						return nullptr;
				}
			}

			for (auto i = g_registeredWSALBackends.begin(); i != g_registeredWSALBackends.end(); ++i) {
				if (!pushedBackends.contains(i.value().get())) {
					deviceCreationQueue.at(deviceCreationQueueBackIndex++) = i.value().get();
				}
			}
		} else {
			size_t deviceCreationQueueBackIndex = 0;
			for (auto i = g_registeredWSALBackends.begin(); i != g_registeredWSALBackends.end(); ++i) {
				deviceCreationQueue.at(deviceCreationQueueBackIndex++) = i.value().get();
			}
		}

		for (size_t i = 0; i < deviceCreationQueue.size(); ++i) {
			auto curBackend = deviceCreationQueue.at(i);
			if (!curBackend->isInited) {
				CLENCH_DEBUG_LOG("WSAL", "Creating window using WSAL backend: %s - skipped due to uninitialized", curBackend->backendId);
				continue;
			}
			CLENCH_DEBUG_LOG("WSAL", "Creating window using WSAL backend: %s", curBackend->backendId);
			if (auto device = curBackend->createWindow(
					flags,
					parent,
					x,
					y,
					width,
					height);
				device) {
				CLENCH_DEBUG_LOG("WSAL", "Created window using WSAL backend: %s", curBackend->backendId);
				return device;
			}
		}
	}

	return nullptr;
}
