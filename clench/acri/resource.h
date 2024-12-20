#ifndef _CLENCH_ACRI_RESOURCE_H_
#define _CLENCH_ACRI_RESOURCE_H_

#include "basedefs.h"
#include <clench/ghal/resource.h>

namespace clench {
	namespace acri {
		class ACRIResource : public utils::RcObject {
		public:
			CLENCH_NO_COPY_MOVE_METHODS(ACRIResource);

			CLCACRI_API ACRIResource();
			CLCACRI_API virtual ~ACRIResource();
		};
	}
}

#endif
