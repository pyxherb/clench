#ifndef _CLENCH_GHAL_BACKENDS_D3D11_TEXTURE_H_
#define _CLENCH_GHAL_BACKENDS_D3D11_TEXTURE_H_

#include "backend.h"
#include <clench/ghal/texture.h>

namespace clench {
	namespace ghal {
		class D3D11Texture1D : public Texture1D {
		public:
			CLENCH_NO_COPY_MOVE_METHODS(D3D11Texture1D);

			ComPtr<ID3D11Texture1D> texture1D;

			CLCGHAL_API D3D11Texture1D(
				Device *ownerDevice,
				const Texture1DDesc &textureDesc,
				ID3D11Texture1D *texture1D
				);
			CLCGHAL_API virtual ~D3D11Texture1D();
		};

		class D3D11Texture2D : public Texture2D {
		public:
			CLENCH_NO_COPY_MOVE_METHODS(D3D11Texture2D);

			ComPtr<ID3D11Texture2D> texture2D;

			CLCGHAL_API D3D11Texture2D(
				Device *ownerDevice,
				const Texture2DDesc &textureDesc,
				ID3D11Texture2D *texture2D);
			CLCGHAL_API virtual ~D3D11Texture2D();
		};

		class D3D11Texture3D : public Texture3D {
		public:
			CLENCH_NO_COPY_MOVE_METHODS(D3D11Texture3D);

			ComPtr<ID3D11Texture3D> texture3D;

			CLCGHAL_API D3D11Texture3D(
				Device *ownerDevice,
				const Texture3DDesc &textureDesc,
				ID3D11Texture3D *texture3D);
			CLCGHAL_API virtual ~D3D11Texture3D();
		};
	}
}

#endif
