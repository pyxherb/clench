#include "device.h"

using namespace clench;
using namespace clench::acri;

CLCACRI_API ACRIDevice::ACRIDevice(
	peff::Alloc *selfAllocator,
	peff::Alloc *resourceAllocator,
	ghal::GHALDevice *associatedDevice)
	: selfAllocator(selfAllocator),
	  resourceAllocator(resourceAllocator),
	  createdResources(selfAllocator),
	  associatedDevice(associatedDevice) {
}

CLCACRI_API ACRIDevice::~ACRIDevice() {
	assert(("Not all ACRI resources are released", false));
}

CLCACRI_API void ACRIDevice::dealloc() {
	peff::deallocAndDestruct<ACRIDevice>(selfAllocator.get(), this, sizeof(std::max_align_t));
}

CLCACRI_API ACRIDeviceContext::ACRIDeviceContext(ACRIDevice *device, ghal::GHALDeviceContext *deviceContext) : ACRIResource(device), deviceContext(deviceContext) {
	assert(("Input device context does not correspond to the device", device->associatedDevice == deviceContext->ownerDevice));
}

CLCACRI_API ACRIDeviceContext::~ACRIDeviceContext() {
}
