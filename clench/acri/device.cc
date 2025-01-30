#include "device.h"
#include "backend.h"

using namespace clench;
using namespace clench::acri;

CLCACRI_API Device::Device(
	ghal::Device *associatedDevice,
	peff::Alloc *selfAllocator,
	peff::Alloc *resourceAllocator)
	: selfAllocator(selfAllocator),
	  resourceAllocator(resourceAllocator),
	  createdResources(selfAllocator),
	  associatedDevice(associatedDevice) {
}

CLCACRI_API Device::~Device() {
	assert(("Not all ACRI resources are released", false));
}

CLCACRI_API DeviceContext::DeviceContext(Device *acriDevice, ghal::DeviceContext *ghalDeviceContext) : ACRIResource(acriDevice), ghalDeviceContext(ghalDeviceContext) {
}

CLCACRI_API DeviceContext::~DeviceContext() {
}

CLCACRI_API void DeviceContext::beginDraw() {
	ghalDeviceContext->begin();
}

CLCACRI_API void DeviceContext::endDraw() {
	ghalDeviceContext->end();
}

CLCACRI_API base::ExceptionPtr clench::acri::createDevice(ghal::Device *ghalDevice, peff::Alloc *selfAllocator, peff::Alloc *resourceAllocator, Device *&deviceOut) {
	if(auto it = g_registeredBackends.find(ghalDevice->getBackend()->backendId); it != g_registeredBackends.end()) {
		return it.value()->createDevice(ghalDevice, selfAllocator, resourceAllocator, deviceOut);
	}

	std::terminate();
}
