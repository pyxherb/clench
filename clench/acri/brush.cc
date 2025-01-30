#include "brush.h"
#include "device.h"

using namespace clench;
using namespace clench::acri;

CLCACRI_API Brush::Brush(Device *device, BrushType brushType) : ACRIResource(device), brushType(brushType) {
}

CLCACRI_API Brush::~Brush() {
}

CLCACRI_API SolidColorBrush::SolidColorBrush(Device *device, const ghal::Color &color): Brush(device, BrushType::SolidColor), color(color) {
}

CLCACRI_API SolidColorBrush::~SolidColorBrush() {
}

CLCACRI_API void SolidColorBrush::dealloc() {
	peff::destroyAndRelease<SolidColorBrush>(device->resourceAllocator.get(), this, sizeof(std::max_align_t));
}

CLCACRI_API SolidColorBrush *SolidColorBrush::alloc(Device *device, const ghal::Color &color) {
	return peff::allocAndConstruct<SolidColorBrush>(device->resourceAllocator.get(), sizeof(std::max_align_t), device, color);
}
