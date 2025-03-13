#include "backend.h"
#include <clench/utils/logger.h>
#include <clench/utils/assert.h>

using namespace clench;
using namespace clench::wsal;

CLCWSAL_API Backend::Backend(const char *backendId, peff::Alloc *selfAllocator, peff::Alloc *resourceAllocator)
	: backendId(backendId),
	  selfAllocator(selfAllocator),
	  resourceAllocator(resourceAllocator) {
}

CLCWSAL_API Backend::~Backend() {
	assert(("The backend has not been deinitialized yet", !isInited));
}

CLCWSAL_API void Backend::onRefZero() noexcept {
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

CLCWSAL_API WSAL::WSAL(peff::Alloc *allocator) : registeredBackends(allocator) {
}

CLCWSAL_API bool WSAL::registerBackend(Backend *backend) {
	return registeredBackends.insert(backend->backendId, BackendPtr(backend));
}

CLCWSAL_API Backend *WSAL::getBackend(const std::string_view &name) {
	if (auto it = registeredBackends.find(name); it != registeredBackends.end()) {
		return it.value().get();
	}
	return nullptr;
}

CLCWSAL_API bool WSAL::unregisterBackend(const std::string_view &name) {
	return registeredBackends.removeAndResizeBuckets(name);
}

CLCWSAL_API void WSAL::enumBackends(void *userData, EnumBackendsProc enumProc) {
	for (auto i = registeredBackends.begin(); i != registeredBackends.end(); ++i) {
		if (!enumProc(userData, i.value().get())) {
			break;
		}
	}
}

CLCWSAL_API size_t WSAL::getBackendNum() {
	return registeredBackends.size();
}
