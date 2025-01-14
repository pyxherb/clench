#ifndef _CLENCH_ACRI_RESOURCE_H_
#define _CLENCH_ACRI_RESOURCE_H_

#include "basedefs.h"
#include <clench/ghal/resource.h>
#include <peff/base/allocator.h>

namespace clench {
	namespace acri {
		class ACRIContext;

		class ACRIResource : public peff::RcObject {
		public:
			ACRIContext *context;

			CLENCH_NO_COPY_MOVE_METHODS(ACRIResource);

			CLCACRI_API ACRIResource(ACRIContext *context);
			CLCACRI_API virtual ~ACRIResource();

			CLCACRI_API virtual void dealloc() = 0;

			CLCACRI_API virtual void onRefZero() noexcept override;
		};
	}
}

#endif
