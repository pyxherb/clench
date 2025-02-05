#ifndef _CLENCH_GHAL_VERTEX_ARRAY_H_
#define _CLENCH_GHAL_VERTEX_ARRAY_H_

#include "resource.h"
#include "shader.h"

namespace clench {
	namespace ghal {
		struct VertexLayoutElementDesc {
			InputVertexShaderSemanticType semanticType;
			uint32_t semanticIndex;
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
