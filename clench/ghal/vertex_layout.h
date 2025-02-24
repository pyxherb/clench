#ifndef _CLENCH_GHAL_VERTEX_ARRAY_H_
#define _CLENCH_GHAL_VERTEX_ARRAY_H_

#include "resource.h"
#include "shader.h"

namespace clench {
	namespace ghal {
		struct VertexLayoutElementDesc {
			/// @brief Semantic type for the input element.
			InputVertexShaderSemanticType semanticType;
			/// @brief Semantic index for the input element, used to form indexed semantics like `POSITION0`.
			uint32_t semanticIndex;
			/// @brief Data type of the input element.
			ShaderDataType dataType;
			size_t stride;
			size_t off;
		};

		class VertexLayout : public DeviceResource {
		public:
			CLENCH_NO_COPY_MOVE_METHODS(VertexLayout);

			CLCGHAL_API VertexLayout(Device* ownerDevice);
			CLCGHAL_API virtual ~VertexLayout();
		};
	}
}

#endif
