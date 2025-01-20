#ifndef _CLENCH_ACRI_BACKENDS_OPENGL_H_
#define _CLENCH_ACRI_BACKENDS_OPENGL_H_

#include <clench/acri/backend.h>
#include <clench/ghal/backends/opengl/backend.h>
#include "device.h"

namespace clench {
	namespace acri {
		class GLACRIBackend : public ACRIBackend {
		public:
			CLCACRI_API GLACRIBackend(peff::Alloc *selfAllocator);
			CLCACRI_API virtual ~GLACRIBackend();

			CLCACRI_API virtual ACRIDevice *createDevice(ghal::GHALDevice *ghalDevice, peff::Alloc *selfAllocator, peff::Alloc *resourceAllocator) override;

			CLCACRI_API static GLACRIBackend *alloc(peff::Alloc *selfAllocator);
		};
	}
}

#endif
