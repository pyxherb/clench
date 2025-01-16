#include "resource.h"
#include "device.h"

using namespace clench;
using namespace clench::acri;

CLCACRI_API ACRIResource::ACRIResource(ACRIDevice *device): device(device) {
}

CLCACRI_API ACRIResource::~ACRIResource() {
}

CLCACRI_API void ACRIResource::onRefZero() noexcept {
	peff::deallocAndDestruct(device->resourceAllocator.get(), this, sizeof(std::max_align_t));
}
