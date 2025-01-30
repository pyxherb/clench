#include "device.h"

using namespace clench;
using namespace clench::acri;

CLCACRI_API ACRIDevice::ACRIDevice(
	ghal::Device *associatedDevice,
	peff::Alloc *selfAllocator,
	peff::Alloc *resourceAllocator)
	: selfAllocator(selfAllocator),
	  resourceAllocator(resourceAllocator),
	  createdResources(selfAllocator),
	  associatedDevice(associatedDevice) {
}

CLCACRI_API ACRIDevice::~ACRIDevice() {
	assert(("Not all ACRI resources are released", false));
}

CLCACRI_API ACRIDeviceContext::ACRIDeviceContext(ACRIDevice *acriDevice, ghal::DeviceContext *ghalDeviceContext) : ACRIResource(acriDevice), ghalDeviceContext(ghalDeviceContext) {
}

CLCACRI_API ACRIDeviceContext::~ACRIDeviceContext() {
}

CLCACRI_API void ACRIDeviceContext::beginDraw() {
	ghalDeviceContext->begin();
}

CLCACRI_API void ACRIDeviceContext::endDraw() {
	ghalDeviceContext->end();
}
