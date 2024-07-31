#ifndef _CLENCH_RHI_BACKENDS_D3D11_SHADER_H_
#define _CLENCH_RHI_BACKENDS_D3D11_SHADER_H_

#include "backend.h"
#include <clench/rhi/shader.h>

namespace clench {
	namespace rhi {
		class D3D11VertexShader : public VertexShader {
		public:
			ComPtr<ID3D11VertexShader> shader;

			CLCRHI_API D3D11VertexShader(RHIDevice *ownedDevice, ID3D11VertexShader *shader);
			CLCRHI_API virtual ~D3D11VertexShader();
		};

		class D3D11FragmentShader : public FragmentShader {
		public:
			ComPtr<ID3D11PixelShader> shader;

			CLCRHI_API D3D11FragmentShader(RHIDevice *ownedDevice, ID3D11PixelShader *shader);
			CLCRHI_API virtual ~D3D11FragmentShader();
		};

		class D3D11GeometryShader : public GeometryShader {
		public:
			ComPtr<ID3D11GeometryShader> shader;

			CLCRHI_API D3D11GeometryShader(RHIDevice *ownedDevice, ID3D11GeometryShader *shader);
			CLCRHI_API virtual ~D3D11GeometryShader();
		};
	}
}

#endif
