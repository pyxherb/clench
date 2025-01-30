#include "device.h"

using namespace clench;
using namespace clench::ghal;

CLCGHAL_API Device::Device(peff::Alloc *selfAllocator, peff::Alloc *resourceAllocator): selfAllocator(selfAllocator), resourceAllocator(resourceAllocator) {
}

CLCGHAL_API Device::~Device() {
	CLENCH_ASSERT(!createdResources.size(), "All GHAL resources should be released before the owner Device deleting");
}

CLCGHAL_API void Device::dealloc() {
	peff::destroyAndRelease<Device>(selfAllocator.get(), this, sizeof(std::max_align_t));
}

CLCGHAL_API DeviceContext::DeviceContext(Device *ownerDevice) : DeviceResource(ownerDevice) {}

CLCGHAL_API DeviceContext::~DeviceContext() {}
