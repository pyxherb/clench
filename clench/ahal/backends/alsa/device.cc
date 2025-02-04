#include "device.h"

#if _WIN32
	#include <clench/wsal/backends/win32/window.h>
#endif

#if __unix__
	#include <clench/wsal/backends/x11/window.h>
#endif

using namespace clench;
using namespace clench::ahal;

CLCAHAL_API ALSADevice::ALSADevice(peff::Alloc *selfAllocator, peff::Alloc *resourceAllocator, ALSABackend *backend)
	: Device(selfAllocator, resourceAllocator),
	  backend(backend) {
}

CLCAHAL_API ALSADevice::~ALSADevice() {
	if(pcmHandle) {
		snd_pcm_drop(pcmHandle);
		snd_pcm_drain(pcmHandle);
		snd_pcm_close(pcmHandle);
	}
	if(periodBuffer) {
		resourceAllocator->release(periodBuffer, szPeriodBuffer, sizeof(std::max_align_t));
	}
}

CLCAHAL_API Backend *ALSADevice::getBackend() {
	return backend;
}

CLCAHAL_API void ALSADevice::dealloc() {
	peff::destroyAndRelease<ALSADevice>(selfAllocator.get(), this, sizeof(std::max_align_t));
}

CLCAHAL_API ALSADevice *ALSADevice::alloc(peff::Alloc *selfAllocator, peff::Alloc *resourceAllocator, ALSABackend *backend) {
	std::unique_ptr<ALSADevice, peff::DeallocableDeleter<ALSADevice>> ptr(
		peff::allocAndConstruct<ALSADevice>(
			selfAllocator, sizeof(std::max_align_t),
			selfAllocator, resourceAllocator, backend));
	if (!ptr)
		return nullptr;

	return (ALSADevice *)ptr.release();
}
