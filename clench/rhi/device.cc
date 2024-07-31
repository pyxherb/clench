#include "device.h"

using namespace clench;
using namespace clench::rhi;

CLCRHI_API RHIDevice::RHIDevice() {
}

CLCRHI_API RHIDevice::~RHIDevice() {
	CLENCH_ASSERT(!createdResources.size(), "All RHI resources should be released before the owner RHIDevice deleting");
}

CLCRHI_API RHIDeviceContext::RHIDeviceContext() {}

CLCRHI_API RHIDeviceContext::~RHIDeviceContext() {}
