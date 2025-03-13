#include "backend.h"
#include <clench/utils/assert.h>
#include <clench/utils/logger.h>

using namespace clench;
using namespace clench::ghal;

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

CLCGHAL_API GHAL::GHAL(peff::Alloc *allocator) : registeredBackends(allocator) {
}

CLCGHAL_API bool GHAL::registerBackend(Backend* backend) {
	return registeredBackends.insert(backend->backendId, BackendPtr(backend));
}

CLCGHAL_API Backend* GHAL::getBackend(const std::string_view& name) {
	if (auto it = registeredBackends.find(name); it != registeredBackends.end()) {
		return it.value().get();
	}
	return nullptr;
}

CLCGHAL_API bool GHAL::unregisterBackend(const std::string_view& name) {
	return registeredBackends.removeAndResizeBuckets(name);
}

CLCGHAL_API void GHAL::enumBackends(void* userData, EnumBackendsProc enumProc) {
	for (auto i = registeredBackends.begin(); i != registeredBackends.end(); ++i) {
		if (!enumProc(userData, i.value().get())) {
			break;
		}
	}
}

CLCGHAL_API size_t GHAL::getBackendNum() {
	return registeredBackends.size();
}
