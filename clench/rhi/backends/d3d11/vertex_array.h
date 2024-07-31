#ifndef _CLENCH_RHI_BACKENDS_D3D11_VERTEX_ARRAY_H_
#define _CLENCH_RHI_BACKENDS_D3D11_VERTEX_ARRAY_H_

#include "backend.h"
#include <clench/rhi/vertex_array.h>

namespace clench {
	namespace rhi {
		class D3D11VertexArray : public VertexArray {
		public:
			ComPtr<ID3D11InputLayout> inputLayout;

			NO_COPY_MOVE_METHODS(D3D11VertexArray);

			CLCRHI_API D3D11VertexArray(
				RHIDevice *ownerDevice,
				ID3D11InputLayout *inputLayout);
			CLCRHI_API virtual ~D3D11VertexArray();
		};
	}
}

#endif
