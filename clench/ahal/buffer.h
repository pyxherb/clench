#ifndef _CLENCH_GHAL_BUFFER_H_
#define _CLENCH_GHAL_BUFFER_H_

#include "resource.h"

namespace clench {
	namespace ahal {
		struct BufferDesc {
			size_t szBuffer;
		};

		class Buffer : public DeviceResource {
		public:
			CLENCH_NO_COPY_MOVE_METHODS(Buffer);

			BufferDesc bufferDesc;

			CLCAHAL_API Buffer(Device *ownerDevice, const BufferDesc &bufferDesc);
			CLCAHAL_API virtual ~Buffer();
		};
	}
}

#endif
