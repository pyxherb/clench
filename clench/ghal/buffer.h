#ifndef _CLENCH_GHAL_BUFFER_H_
#define _CLENCH_GHAL_BUFFER_H_

#include "resource.h"

namespace clench {
	namespace ghal {
		enum class BufferUsage {
			Default = 0,
			Static,
			Dynamic,
			Staging
		};

		enum class BufferTarget {
			Vertex = 0,
			Index,
			UniformBuffer
		};

		struct BufferDesc {
			size_t size;
			BufferUsage usage;
			BufferTarget proposedTarget;
			bool cpuReadable;
			bool cpuWritable;
		};

		class Buffer : public DeviceResource {
		public:
			CLENCH_NO_COPY_MOVE_METHODS(Buffer);

			BufferDesc bufferDesc;

			CLCGHAL_API Buffer(Device *ownerDevice, const BufferDesc &bufferDesc);
			CLCGHAL_API virtual ~Buffer();
		};
	}
}

#endif
