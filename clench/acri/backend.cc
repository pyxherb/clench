#include "backend.h"

using namespace clench;
using namespace clench::acri;

CLCACRI_API peff::HashMap<std::string_view, peff::RcObjectPtr<Backend>> clench::acri::g_registeredBackends;

CLCACRI_API Backend::Backend(const char *name, peff::Alloc *selfAllocator) : name(name), selfAllocator(selfAllocator) {
}

CLCACRI_API Backend::~Backend() {
}

CLCACRI_API void Backend::onRefZero() noexcept {
	dealloc();
}

CLCACRI_API bool clench::acri::registerBackend(Backend *backend) {
	return g_registeredBackends.insert((std::string_view)backend->name, backend);
}

CLCACRI_API bool clench::acri::unregisterBackend(std::string_view name) {
	g_registeredBackends.remove(name);
}
