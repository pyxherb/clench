#ifndef _CLENCH_GHAL_VERTEX_ARRAY_H_
#define _CLENCH_GHAL_VERTEX_ARRAY_H_

#include "resource.h"
#include "shader.h"

namespace clench {
	namespace ghal {
		enum class VertexElementType : uint8_t {
			Int = 0,
			UInt,
			Short,
			UShort,
			Long,
			ULong,
			Float,
			Double,
			Boolean,
			Half
		};

		struct VertexDataType {
			VertexElementType elementType;
			uint32_t nElements;

			PEFF_FORCEINLINE bool operator<(const VertexDataType &rhs) {
				if(elementType < rhs.elementType)
					return true;
				if(elementType > rhs.elementType)
					return false;
				return nElements < rhs.nElements;
			}
		};

		struct VertexArrayElementDesc {
			InputVertexShaderSemanticType semanticType;
			uint32_t semanticIndex;
			VertexDataType dataType;
			uint32_t stride;
			uint32_t off;
		};

		class VertexArray : public GHALDeviceResource {
		public:
			CLENCH_NO_COPY_MOVE_METHODS(VertexArray);

			CLCGHAL_API VertexArray(GHALDevice* ownerDevice);
			CLCGHAL_API virtual ~VertexArray();
		};
	}
}

#endif
