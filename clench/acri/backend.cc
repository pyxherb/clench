#include "backend.h"

using namespace clench;
using namespace clench::acri;

CLCACRI_API peff::HashMap<std::string_view, peff::RcObjectPtr<ACRIBackend>> clench::acri::g_registeredBackends;

CLCACRI_API ACRIBackend::ACRIBackend(const char *name, peff::Alloc *selfAllocator) : name(name), selfAllocator(selfAllocator) {
}

CLCACRI_API ACRIBackend::~ACRIBackend() {
}

CLCACRI_API void ACRIBackend::onRefZero() noexcept {
	dealloc();
}

CLCACRI_API bool clench::acri::registerBackend(ACRIBackend *backend) {
	return g_registeredBackends.insert((std::string_view)backend->name, backend);
}

CLCACRI_API bool clench::acri::unregisterBackend(std::string_view name) {
	g_registeredBackends.remove(name);
}
