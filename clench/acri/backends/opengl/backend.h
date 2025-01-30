#ifndef _CLENCH_ACRI_BACKENDS_OPENGL_BACKEND_H_
#define _CLENCH_ACRI_BACKENDS_OPENGL_BACKEND_H_

#include <clench/acri/backend.h>
#include <clench/ghal/backends/opengl/backend.h>
#include "device.h"

namespace clench {
	namespace acri {
		class GLBackend : public Backend {
		public:
			CLCACRI_API GLBackend(peff::Alloc *selfAllocator);
			CLCACRI_API virtual ~GLBackend();

			CLCACRI_API virtual base::ExceptionPtr createDevice(
				ghal::Device *ghalDevice,
				peff::Alloc *selfAllocator,
				peff::Alloc *resourceAllocator,
				Device *&deviceOut) override;

			CLCACRI_API virtual void dealloc() override;

			CLCACRI_API static GLBackend *alloc(peff::Alloc *selfAllocator);
		};
	}
}

#endif
