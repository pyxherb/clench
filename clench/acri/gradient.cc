#include "gradient.h"
#include "device.h"

using namespace clench;
using namespace clench::acri;

CLCACRI_API Gradient::Gradient(Device *device, GradientType gradientType) : ACRIResource(device), gradientType(gradientType) {
}

CLCACRI_API Gradient::~Gradient() {
}

CLCACRI_API LinearGradient::LinearGradient(Device *device) : Gradient(device, GradientType::Linear), colorStops(device->resourceAllocator.get()) {
}

CLCACRI_API LinearGradient::~LinearGradient() {
}

CLCACRI_API void LinearGradient::dealloc() {
	peff::destroyAndRelease<LinearGradient>(device->resourceAllocator.get(), this, sizeof(std::max_align_t));
}
