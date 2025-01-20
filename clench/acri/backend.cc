#include "backend.h"

using namespace clench;
using namespace clench::acri;

CLCACRI_API peff::HashMap<peff::String, peff::RcObjectPtr<ACRIBackend>> clench::acri::g_registeredBackends;

CLCACRI_API ACRIBackend::ACRIBackend(const char *name, peff::Alloc *selfAllocator) : name(name), selfAllocator(selfAllocator) {
}

CLCACRI_API ACRIBackend::~ACRIBackend() {
}

CLCACRI_API void ACRIBackend::onRefZero() noexcept {
	peff::destroyAndRelease(selfAllocator.get(), this, sizeof(std::max_align_t));
}

CLCACRI_API bool clench::acri::registerBackend(ACRIBackend *backend) {
	peff::String s;

	{
		size_t nameLen = strlen(backend->name);
		if(!s.resize(nameLen))
			return false;
		memcpy(s.data(), backend->name, nameLen);
	}

	return g_registeredBackends.insert(std::move(s), backend);
}
