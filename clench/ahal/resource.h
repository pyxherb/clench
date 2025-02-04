#ifndef _CLENCH_AHAL_RESOURCE_H_
#define _CLENCH_AHAL_RESOURCE_H_

#include <peff/base/rcobj.h>
#include <peff/base/deallocable.h>
#include "basedefs.h"

namespace clench {
	namespace ahal {
		class Device;

		class DeviceResource : public peff::RcObject {
		public:
			Device *ownerDevice;

			CLENCH_NO_COPY_MOVE_METHODS(DeviceResource);

			CLCAHAL_API DeviceResource(Device *ownerDevice);
			CLCAHAL_API virtual ~DeviceResource();

			virtual void dealloc() = 0;

			CLCAHAL_API virtual void onRefZero() noexcept override;
		};
	}
}

#endif
