#include "device.h"

using namespace clench;
using namespace clench::ghal;

CLCGHAL_API GHALDevice::GHALDevice(peff::Alloc *selfAllocator, peff::Alloc *resourceAllocator): selfAllocator(selfAllocator), resourceAllocator(resourceAllocator) {
}

CLCGHAL_API GHALDevice::~GHALDevice() {
	CLENCH_ASSERT(!createdResources.size(), "All GHAL resources should be released before the owner GHALDevice deleting");
}

CLCGHAL_API void GHALDevice::dealloc() {
	peff::destroyAndRelease<GHALDevice>(selfAllocator.get(), this, sizeof(std::max_align_t));
}

CLCGHAL_API GHALDeviceContext::GHALDeviceContext(GHALDevice *ownerDevice) : GHALDeviceResource(ownerDevice) {}

CLCGHAL_API GHALDeviceContext::~GHALDeviceContext() {}
