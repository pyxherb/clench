#ifndef _CLENCH_ACRI_CONTEXT_H_
#define _CLENCH_ACRI_CONTEXT_H_

#include "resource.h"
#include <peff/base/alloc.h>
#include <peff/containers/set.h>
#include <clench/ghal/device.h>

namespace clench {
	namespace acri {
		class ACRIDevice : public peff::Deallocable {
		public:
			peff::RcObjectPtr<peff::Alloc> selfAllocator, resourceAllocator;
			ghal::GHALDevice *associatedDevice;
			peff::Set<ACRIResource *> createdResources;

			CLCACRI_API ACRIDevice(peff::Alloc *selfAllocator, peff::Alloc *resourceAllocator, ghal::GHALDevice *associatedDevice);
			CLCACRI_API ~ACRIDevice();

			CLCACRI_API virtual void dealloc() override;
		};

		class ACRIDeviceContext : public ACRIResource {
		public:
			peff::RcObjectPtr<ghal::GHALDeviceContext> deviceContext;

			CLCACRI_API ACRIDeviceContext(ACRIDevice *device, ghal::GHALDeviceContext *deviceContext);
			CLCACRI_API ~ACRIDeviceContext();
		};
	}
}

#endif
