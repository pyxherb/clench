#ifndef _CLENCH_GHAL_BACKENDS_D3D11_BUFFER_H_
#define _CLENCH_GHAL_BACKENDS_D3D11_BUFFER_H_

#include "backend.h"
#include <clench/ghal/buffer.h>

namespace clench {
	namespace ghal {
		class D3D11Buffer : public Buffer {
		public:
			ComPtr<ID3D11Buffer> buffer;

			NO_COPY_MOVE_METHODS(D3D11Buffer);

			CLCGHAL_API D3D11Buffer(
				GHALDevice *ownerDevice,
				const BufferDesc &bufferDesc,
				ID3D11Buffer *buffer);
			CLCGHAL_API virtual ~D3D11Buffer();
		};
	}
}

#endif
