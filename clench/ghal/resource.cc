#include "device.h"

using namespace clench;
using namespace clench::ghal;

CLCGHAL_API GHALDeviceResource::GHALDeviceResource(GHALDevice* ownerDevice) : RcObject(), ownerDevice(ownerDevice) {
	ownerDevice->createdResources.insert(this);
}

CLCGHAL_API GHALDeviceResource::~GHALDeviceResource() {
	ownerDevice->createdResources.erase(this);
}

CLCGHAL_API void GHALDeviceResource::onRefZero() noexcept {
	peff::RcObjectPtr<peff::Alloc> allocator = ownerDevice->resourceAllocator;
	std::destroy_at<GHALDeviceResource>(this);

	allocator->release(this);
}
