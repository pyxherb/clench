#ifndef _CLENCH_GHAL_RESOURCE_H_
#define _CLENCH_GHAL_RESOURCE_H_

#include <peff/base/rcobj.h>
#include <peff/base/deallocable.h>
#include "basedefs.h"

namespace clench {
	namespace ghal {
		class Device;

		class DeviceResource : public peff::RcObject {
		public:
			Device *ownerDevice;

			CLENCH_NO_COPY_MOVE_METHODS(DeviceResource);

			CLCGHAL_API DeviceResource(Device *ownerDevice);
			CLCGHAL_API virtual ~DeviceResource();

			virtual void dealloc() = 0;

			CLCGHAL_API virtual void onRefZero() noexcept override;
		};
	}
}

#endif
