#include "backend.h"

using namespace clench;
using namespace clench::acri;

CLCACRI_API GLACRIBackend::GLACRIBackend(peff::Alloc *selfAllocator) : ACRIBackend("opengl", selfAllocator) {
}

CLCACRI_API GLACRIBackend::~GLACRIBackend() {
}

CLCACRI_API ACRIDevice *createDevice(ghal::GHALDevice *ghalDevice, peff::Alloc *selfAllocator, peff::Alloc *resourceAllocator) {

}

CLCACRI_API GLACRIBackend *GLACRIBackend::alloc(peff::Alloc *selfAllocator) {
	return peff::allocAndConstruct<GLACRIBackend>(selfAllocator, sizeof(std::max_align_t), selfAllocator);
}
