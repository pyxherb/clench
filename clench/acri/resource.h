#ifndef _CLENCH_ACRI_RESOURCE_H_
#define _CLENCH_ACRI_RESOURCE_H_

#include "basedefs.h"
#include <clench/ghal/resource.h>
#include <peff/base/alloc.h>

namespace clench {
	namespace acri {
		class Device;

		class ACRIResource : public peff::RcObject {
		public:
			Device *device;

			CLENCH_NO_COPY_MOVE_METHODS(ACRIResource);

			CLCACRI_API ACRIResource(Device *device);
			CLCACRI_API virtual ~ACRIResource();

			virtual void dealloc() = 0;

			CLCACRI_API virtual void onRefZero() noexcept override;
		};
	}
}

#endif
