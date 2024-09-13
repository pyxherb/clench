#ifndef _CLENCH_GHAL_BACKENDS_D3D11_SHADER_H_
#define _CLENCH_GHAL_BACKENDS_D3D11_SHADER_H_

#include "backend.h"
#include <clench/ghal/shader.h>

namespace clench {
	namespace ghal {
		class D3D11VertexShader : public VertexShader {
		public:
			ComPtr<ID3D11VertexShader> shader;
			std::unique_ptr<char[]> source;
			size_t szSource;

			CLCGHAL_API D3D11VertexShader(
				GHALDevice *ownedDevice,
				ID3D11VertexShader *shader,
				const char *source,
				size_t szSource);
			CLCGHAL_API virtual ~D3D11VertexShader();
		};

		class D3D11FragmentShader : public FragmentShader {
		public:
			ComPtr<ID3D11PixelShader> shader;

			CLCGHAL_API D3D11FragmentShader(GHALDevice *ownedDevice, ID3D11PixelShader *shader);
			CLCGHAL_API virtual ~D3D11FragmentShader();
		};

		class D3D11GeometryShader : public GeometryShader {
		public:
			ComPtr<ID3D11GeometryShader> shader;

			CLCGHAL_API D3D11GeometryShader(GHALDevice *ownedDevice, ID3D11GeometryShader *shader);
			CLCGHAL_API virtual ~D3D11GeometryShader();
		};
	}
}

#endif
