#ifndef _CLENCH_GHAL_COMMAND_H_
#define _CLENCH_GHAL_COMMAND_H_

#include "device.h"
#include <cstdint>
#include <map>

namespace clench {
	namespace ghal {
		class GHALCommandList : public GHALDeviceResource {
		public:
			CLENCH_NO_COPY_MOVE_METHODS(GHALCommandList);

			CLCGHAL_API GHALCommandList(GHALDevice *ownerDevice);
			CLCGHAL_API virtual ~GHALCommandList();
		};
	}
}

#endif
