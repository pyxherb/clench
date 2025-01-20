#ifndef _CLENCH_GHAL_BACKENDS_D3D11_VERTEX_ARRAY_H_
#define _CLENCH_GHAL_BACKENDS_D3D11_VERTEX_ARRAY_H_

#include "backend.h"
#include <clench/ghal/vertex_array.h>

namespace clench {
	namespace ghal {
		class D3D11VertexLayout : public VertexLayout {
		public:
			ComPtr<ID3D11InputLayout> inputLayout;

			CLENCH_NO_COPY_MOVE_METHODS(D3D11VertexLayout);

			CLCGHAL_API D3D11VertexLayout(
				GHALDevice *ownerDevice,
				ID3D11InputLayout *inputLayout);
			CLCGHAL_API virtual ~D3D11VertexLayout();
		};
	}
}

#endif
