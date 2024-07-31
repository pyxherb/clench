#include "device.h"

using namespace clench;
using namespace clench::rhi;

CLCRHI_API RHIDeviceResource::RHIDeviceResource(RHIDevice* device) : RcObject(), ownerDevice(ownerDevice) {
	device->createdResources.insert(this);
}

CLCRHI_API RHIDeviceResource::~RHIDeviceResource() {
	ownerDevice->createdResources.erase(this);
}