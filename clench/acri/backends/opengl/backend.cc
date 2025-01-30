#include "backend.h"

using namespace clench;
using namespace clench::acri;

CLCACRI_API GLACRIBackend::GLACRIBackend(peff::Alloc *selfAllocator) : ACRIBackend("opengl", selfAllocator) {
}

CLCACRI_API GLACRIBackend::~GLACRIBackend() {
}

CLCACRI_API ACRIDevice *createDevice(ghal::Device *ghalDevice, peff::Alloc *selfAllocator, peff::Alloc *resourceAllocator) {

}

CLCACRI_API void GLACRIBackend::dealloc() {
	peff::destroyAndRelease<GLACRIBackend>(selfAllocator.get(), this, sizeof(std::max_align_t));
}

CLCACRI_API GLACRIBackend *GLACRIBackend::alloc(peff::Alloc *selfAllocator) {
	return peff::allocAndConstruct<GLACRIBackend>(selfAllocator, sizeof(std::max_align_t), selfAllocator);
}
