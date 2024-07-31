#ifndef _CLENCH_RHI_BACKENDS_D3D11_BACKEND_H_
#define _CLENCH_RHI_BACKENDS_D3D11_BACKEND_H_

#include <clench/rhi/backend.h>
#include <d3d11_1.h>
#include <wrl.h>

namespace clench {
	namespace rhi {
		using namespace Microsoft::WRL;

		class D3D11RHIBackend : public RHIBackend {
		public:
			NO_COPY_MOVE_METHODS(D3D11RHIBackend);

			CLCRHI_API D3D11RHIBackend();
			CLCRHI_API virtual ~D3D11RHIBackend();

			CLCRHI_API virtual RHIDevice* createDevice() override;
		};
	}
}

#endif
