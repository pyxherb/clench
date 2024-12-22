#include "device.h"

using namespace clench;
using namespace clench::acri;

CLCACRI_API ACRIDevice::ACRIDevice() {
}

CLCACRI_API ACRIDevice::~ACRIDevice() {
	assert(("Not all ACRI resources are released", false));
}
