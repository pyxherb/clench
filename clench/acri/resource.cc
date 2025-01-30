#include "resource.h"
#include "device.h"

using namespace clench;
using namespace clench::acri;

CLCACRI_API ACRIResource::ACRIResource(Device *device): device(device) {
}

CLCACRI_API ACRIResource::~ACRIResource() {
}

CLCACRI_API void ACRIResource::onRefZero() noexcept {
	dealloc();
}
