#ifndef _CLENCH_GHAL_RESOURCE_H_
#define _CLENCH_GHAL_RESOURCE_H_

#include <clench/utils/rcobj.h>
#include "basedefs.h"

namespace clench {
	namespace ghal {
		class GHALDevice;

		class GHALDeviceResource : public utils::RcObject {
		public:
			GHALDevice* ownerDevice;

			NO_COPY_MOVE_METHODS(GHALDeviceResource);

			CLCGHAL_API GHALDeviceResource(GHALDevice* ownerDevice);
			CLCGHAL_API virtual ~GHALDeviceResource();
		};
	}
}

#endif
