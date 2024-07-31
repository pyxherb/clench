#ifndef _CLENCH_RHI_BUFFER_H_
#define _CLENCH_RHI_BUFFER_H_

#include "basedefs.h"

namespace clench {
	namespace rhi {
		enum class BufferVolatility {
		};

		enum class BufferUsage {
			Default = 0,
			Static,
			Dynamic,
			Staging
		};

		enum class BufferType {
			Vertex = 0,
			Index,
		};

		struct BufferDesc {
			BufferUsage usage;
			BufferType type;
		};

		class Buffer {
		public:
			BufferDesc bufferDesc;

			CLCRHI_API Buffer(const BufferDesc &desc);
			CLCRHI_API virtual ~Buffer();
		};
	}
}

#endif