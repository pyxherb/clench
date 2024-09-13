#include "device.h"

using namespace clench;
using namespace clench::ghal;

CLCGHAL_API GHALDevice::GHALDevice() {
}

CLCGHAL_API GHALDevice::~GHALDevice() {
	CLENCH_ASSERT(!createdResources.size(), "All GHAL resources should be released before the owner GHALDevice deleting");
}

CLCGHAL_API GHALDeviceContext::GHALDeviceContext() {}

CLCGHAL_API GHALDeviceContext::~GHALDeviceContext() {}
