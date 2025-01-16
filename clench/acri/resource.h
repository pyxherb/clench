#ifndef _CLENCH_ACRI_RESOURCE_H_
#define _CLENCH_ACRI_RESOURCE_H_

#include "basedefs.h"
#include <clench/ghal/resource.h>
#include <peff/base/allocator.h>

namespace clench {
	namespace acri {
		class ACRIDevice;

		class ACRIResource : public peff::RcObject {
		public:
			ACRIDevice *device;

			CLENCH_NO_COPY_MOVE_METHODS(ACRIResource);

			CLCACRI_API ACRIResource(ACRIDevice *device);
			CLCACRI_API virtual ~ACRIResource();

			CLCACRI_API virtual void onRefZero() noexcept override;
		};
	}
}

#endif
