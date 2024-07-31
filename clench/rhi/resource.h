#ifndef _CLENCH_RHI_RESOURCE_H_
#define _CLENCH_RHI_RESOURCE_H_

#include <clench/utils/rcobj.h>
#include "basedefs.h"

namespace clench {
	namespace rhi {
		class RHIDevice;

		class RHIDeviceResource : public utils::RcObject {
		public:
			RHIDevice* ownerDevice;

			NO_COPY_MOVE_METHODS(RHIDeviceResource);

			CLCRHI_API RHIDeviceResource(RHIDevice* ownedDevice);
			CLCRHI_API virtual ~RHIDeviceResource();
		};
	}
}

#endif