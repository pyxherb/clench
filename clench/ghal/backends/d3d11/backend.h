#ifndef _CLENCH_GHAL_BACKENDS_D3D11_BACKEND_H_
#define _CLENCH_GHAL_BACKENDS_D3D11_BACKEND_H_

#include <clench/ghal/backend.h>
#include <d3d11_1.h>
#include <wrl.h>

namespace clench {
	namespace ghal {
		using namespace Microsoft::WRL;

		CLCGHAL_API extern const char g_clearRtvVertShaderByteCode[];
		CLCGHAL_API extern const size_t g_clearRtvVertShaderByteCode_length;

		CLCGHAL_API extern const char g_clearRtvFragShaderByteCode[];
		CLCGHAL_API extern const size_t g_clearRtvFragShaderByteCode_length;

		class D3D11GHALBackend : public GHALBackend {
		public:
			NO_COPY_MOVE_METHODS(D3D11GHALBackend);

			CLCGHAL_API D3D11GHALBackend();
			CLCGHAL_API virtual ~D3D11GHALBackend();

			CLCGHAL_API virtual GHALDevice* createDevice() override;
		};
	}
}

#endif
