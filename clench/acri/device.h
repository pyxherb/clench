#ifndef _CLENCH_ACRI_CONTEXT_H_
#define _CLENCH_ACRI_CONTEXT_H_

#include "resource.h"
#include <set>

namespace clench {
	namespace acri {
		class ACRIDevice {
		public:
			std::pmr::set<ACRIResource *> createdResources;

			CLCACRI_API ACRIDevice();
			CLCACRI_API ~ACRIDevice();
		};
	}
}

#endif
