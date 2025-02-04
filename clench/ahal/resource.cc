#include "device.h"

using namespace clench;
using namespace clench::ahal;

CLCAHAL_API DeviceResource::DeviceResource(Device* ownerDevice) : RcObject(), ownerDevice(ownerDevice) {
	ownerDevice->createdResources.insert(this);
}

CLCAHAL_API DeviceResource::~DeviceResource() {
	ownerDevice->createdResources.erase(this);
}

CLCAHAL_API void DeviceResource::onRefZero() noexcept {
	this->dealloc();
}
