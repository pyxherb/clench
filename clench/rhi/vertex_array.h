#ifndef _CLENCH_RHI_VERTEX_ARRAY_H_
#define _CLENCH_RHI_VERTEX_ARRAY_H_

#include "resource.h"
#include "shader.h"

namespace clench {
	namespace rhi {
		enum class VertexDataType : uint8_t {
			Int,
			UInt,
			Short,
			UShort,
			Long,
			ULong,
			Float,
			Double,
			Boolean
		};

		struct VertexArrayElementDesc {
			InputVertexShaderSemanticType semanticType;
			uint32_t semanticIndex;
			uint32_t index;
			uint32_t nElements;
			VertexDataType dataType;
			uint32_t stride;
			uint32_t off;
		};

		class VertexArray : public RHIDeviceResource {
		public:
			NO_COPY_MOVE_METHODS(VertexArray);

			CLCRHI_API VertexArray(RHIDevice* ownerDevice);
			CLCRHI_API virtual ~VertexArray();
		};
	}
}

#endif
