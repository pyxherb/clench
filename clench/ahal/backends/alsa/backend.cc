#include "backend.h"
#include "device.h"

using namespace clench;
using namespace clench::ahal;

#if _WIN32
CLCAHAL_API HMODULE clench::ahal::g_hOpenGL32Dll = NULL;
#elif __unix__
#endif
CLCAHAL_API ALSABackend *clench::ahal::g_alsaBackend = nullptr;

CLCAHAL_API ALSABackend::ALSABackend(peff::Alloc *selfAllocator) : Backend("opengl", selfAllocator) {
	g_alsaBackend = this;
}

CLCAHAL_API ALSABackend::~ALSABackend() {
}

CLCAHAL_API void ALSABackend::dealloc() {
	peff::destroyAndRelease<ALSABackend>(selfAllocator.get(), this, sizeof(std::max_align_t));
}

bool ALSABackend::doInit() {
	return true;
}

bool ALSABackend::doDeinit() {
	return true;
}

CLCAHAL_API base::ExceptionPtr ALSABackend::createDevice(peff::Alloc *allocator, Device *&ahalDeviceOut) {
	std::unique_ptr<ALSADevice, peff::DeallocableDeleter<ALSADevice>> ptr(ALSADevice::alloc(allocator, allocator, this));
	if (!ptr)
		return base::OutOfMemoryException::alloc();

	if (snd_pcm_open(&ptr->pcmHandle, "default", SND_PCM_STREAM_PLAYBACK, 0) < 0) {
		return base::wrapIfExceptAllocFailed(ErrorOpeningDeviceException::alloc(allocator));
	}

	snd_pcm_hw_params_t *hwParams;

	snd_pcm_hw_params_alloca(&hwParams);

	snd_pcm_hw_params_any(ptr->pcmHandle, hwParams);

	snd_pcm_hw_params_set_access(ptr->pcmHandle, hwParams, SND_PCM_ACCESS_RW_INTERLEAVED);

	snd_pcm_hw_params_set_format(ptr->pcmHandle, hwParams, SND_PCM_FORMAT_S16);

	snd_pcm_hw_params_set_channels(ptr->pcmHandle, hwParams, 2);

	unsigned int samplePerSecond = 48000;
	int dir;

	snd_pcm_hw_params_set_rate_near(ptr->pcmHandle, hwParams, &samplePerSecond, &dir);

	snd_pcm_uframes_t frames = 60;

	snd_pcm_hw_params_set_period_size_near(ptr->pcmHandle, hwParams, &frames, &dir);

	if (snd_pcm_hw_params(ptr->pcmHandle, hwParams) < 0) {
		return base::wrapIfExceptAllocFailed(ErrorOpeningDeviceException::alloc(allocator));
	}

	snd_pcm_hw_params_get_period_size(hwParams, &frames, &dir);

	size_t size = frames * sizeof(uint16_t) * 2;

	ptr->periodBuffer = allocator->alloc(size, sizeof(std::max_align_t));
	if(!ptr->periodBuffer)
		return base::OutOfMemoryException::alloc();
	ptr->szPeriodBuffer = size;

	ahalDeviceOut = ptr.release();
	return {};
}

CLCAHAL_API ALSABackend *ALSABackend::alloc(peff::Alloc *selfAllocator) {
	return peff::allocAndConstruct<ALSABackend>(selfAllocator, sizeof(std::max_align_t), selfAllocator);
}
