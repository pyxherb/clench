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
	peff::deallocAndDestruct<GHALDeviceResource>(ownerDevice->resourceAllocator.get(), this, sizeof(std::max_align_t));
}
