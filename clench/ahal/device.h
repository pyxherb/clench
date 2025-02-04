#ifndef _CLENCH_AHAL_DEVICE_H_
#define _CLENCH_AHAL_DEVICE_H_

#include "backend.h"
#include "resource.h"
#include <clench/utils/assert.h>
#include <clench/wsal/window.h>
#include <set>
#include <peff/base/alloc.h>

namespace clench {
	namespace ahal {
		class DeviceContext;

		class Device {
		public:
			peff::RcObjectPtr<peff::Alloc> selfAllocator, resourceAllocator;
			std::set<DeviceResource *> createdResources;

			CLENCH_NO_COPY_MOVE_METHODS(Device);

			CLCAHAL_API Device(peff::Alloc *selfAllocator, peff::Alloc *resourceAllocator);
			CLCAHAL_API virtual ~Device();

			virtual void dealloc() = 0;

			virtual Backend *getBackend() = 0;
		};
	}
}

#endif
