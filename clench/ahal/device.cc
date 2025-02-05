#include "device.h"

using namespace clench;
using namespace clench::ahal;

CLCAHAL_API Device::Device(peff::Alloc *selfAllocator, peff::Alloc *resourceAllocator): selfAllocator(selfAllocator), resourceAllocator(resourceAllocator) {
}

CLCAHAL_API Device::~Device() {
	CLENCH_ASSERT(!createdResources.size(), "All AHAL resources should be released before the owner Device deleting");
}

CLCAHAL_API void Device::dealloc() {
	peff::destroyAndRelease<Device>(selfAllocator.get(), this, sizeof(std::max_align_t));
}
