#ifndef _CLENCH_GHAL_BACKENDS_D3D11_BACKEND_H_
#define _CLENCH_GHAL_BACKENDS_D3D11_BACKEND_H_

#include <clench/ghal/backend.h>
#include <d3d11_1.h>
#include <wrl.h>

namespace clench {
	namespace ghal {
		using namespace Microsoft::WRL;

		class D3D11GHALBackend : public GHALBackend {
		public:
			CLENCH_NO_COPY_MOVE_METHODS(D3D11GHALBackend);

			CLCGHAL_API D3D11GHALBackend();
			CLCGHAL_API virtual ~D3D11GHALBackend();

			CLCGHAL_API virtual GHALDevice* createDevice() override;
		};
	}
}

#endif
