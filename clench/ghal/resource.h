#ifndef _CLENCH_GHAL_RESOURCE_H_
#define _CLENCH_GHAL_RESOURCE_H_

#include <peff/base/rcobj.h>
#include <peff/base/deallocable.h>
#include "basedefs.h"

namespace clench {
	namespace ghal {
		class GHALDevice;

		class GHALDeviceResource : public peff::RcObject {
		public:
			GHALDevice *ownerDevice;

			CLENCH_NO_COPY_MOVE_METHODS(GHALDeviceResource);

			CLCGHAL_API GHALDeviceResource(GHALDevice *ownerDevice);
			CLCGHAL_API virtual ~GHALDeviceResource();

			virtual void dealloc() = 0;

			CLCGHAL_API virtual void onRefZero() noexcept override;
		};
	}
}

#endif
