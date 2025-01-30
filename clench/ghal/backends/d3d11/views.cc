#include "views.h"

using namespace clench;
using namespace clench::ghal;

CLCGHAL_API D3D11RenderTargetView::D3D11RenderTargetView(
	Device *ownerDevice,
	RenderTargetViewType renderTargetViewType,
	ID3D11RenderTargetView *renderTargetView)
	: RenderTargetView(ownerDevice, renderTargetViewType),
	  renderTargetView(renderTargetView) {
}

CLCGHAL_API D3D11RenderTargetView::~D3D11RenderTargetView() {
}

CLCGHAL_API D3D11DepthStencilView::D3D11DepthStencilView(
	Device *ownerDevice,
	DepthStencilViewType depthStencilViewType,
	ID3D11DepthStencilView *depthStencilView)
	: DepthStencilView(ownerDevice, depthStencilViewType),
	  depthStencilView(depthStencilView) {
}

CLCGHAL_API D3D11DepthStencilView::~D3D11DepthStencilView() {
}
