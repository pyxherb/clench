#include "device.h"

using namespace clench;
using namespace clench::ghal;

CLCGHAL_API DeviceResource::DeviceResource(Device* ownerDevice) : RcObject(), ownerDevice(ownerDevice) {
	ownerDevice->createdResources.insert(this);
}

CLCGHAL_API DeviceResource::~DeviceResource() {
	ownerDevice->createdResources.erase(this);
}

CLCGHAL_API void DeviceResource::onRefZero() noexcept {
	this->dealloc();
}
