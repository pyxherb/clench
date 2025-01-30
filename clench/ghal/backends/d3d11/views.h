#ifndef _CLENCH_GHAL_BACKENDS_D3D11_VIEWS_H_
#define _CLENCH_GHAL_BACKENDS_D3D11_VIEWS_H_

#include "backend.h"
#include <clench/ghal/views.h>

namespace clench {
	namespace ghal {
		class D3D11RenderTargetView : public RenderTargetView {
		public:
			CLENCH_NO_COPY_MOVE_METHODS(D3D11RenderTargetView);

			ComPtr<ID3D11RenderTargetView> renderTargetView;

			CLCGHAL_API D3D11RenderTargetView(
				Device *ownerDevice,
				RenderTargetViewType renderTargetViewType,
				ID3D11RenderTargetView *renderTargetView);
			CLCGHAL_API virtual ~D3D11RenderTargetView();
		};

		class D3D11DepthStencilView : public DepthStencilView {
		public:
			CLENCH_NO_COPY_MOVE_METHODS(D3D11DepthStencilView);

			ComPtr<ID3D11DepthStencilView> depthStencilView;

			CLCGHAL_API D3D11DepthStencilView(
				Device *ownerDevice,
				DepthStencilViewType depthStencilViewType,
				ID3D11DepthStencilView *depthStencilView);
			CLCGHAL_API virtual ~D3D11DepthStencilView();
		};
	}
}

#endif
