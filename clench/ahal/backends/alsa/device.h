#ifndef _CLENCH_AHAL_BACKENDS_OPENGL_DEVICE_H_
#define _CLENCH_AHAL_BACKENDS_OPENGL_DEVICE_H_

#include "except.h"
#include "backend.h"
// #include "buffer.h"
#include <clench/ahal/device.h>
#include <thread>
#include <optional>
#include <mutex>
#include <clench/utils/logger.h>

namespace clench {
	namespace ahal {
		class ALSADeviceContext;

		class ALSADevice : public Device {
		public:
			ALSABackend *backend;

			snd_pcm_t *pcmHandle;

			void *periodBuffer;
			size_t szPeriodBuffer;

			/// @brief Default context for some internal operations. DO NOT try to draw with it!
			/// @note DO NOT forget to create one for the device after the creation!
			peff::RcObjectPtr<ALSADeviceContext> defaultContext;

			CLENCH_NO_COPY_MOVE_METHODS(ALSADevice);

			CLCAHAL_API ALSADevice(peff::Alloc *selfAllocator, peff::Alloc *resourceAllocator, ALSABackend *backend);
			CLCAHAL_API virtual ~ALSADevice();

			CLCAHAL_API virtual Backend *getBackend() override;

			CLCAHAL_API virtual void dealloc() override;

			CLCAHAL_API static ALSADevice *alloc(peff::Alloc *selfAllocator, peff::Alloc *resourceAllocator, ALSABackend *backend);
		};
	}
}

#endif
