#include "views.h"

using namespace clench;
using namespace clench::ghal;

CLCGHAL_API RenderTargetView::RenderTargetView(
	Device *ownerDevice,
	RenderTargetViewType renderTargetViewType)
	: DeviceResource(ownerDevice), renderTargetViewType(renderTargetViewType) {
}

CLCGHAL_API RenderTargetView::~RenderTargetView() {}

CLCGHAL_API DepthStencilView::DepthStencilView(
	Device* ownerDevice,
	DepthStencilViewType depthStencilViewType
) : DeviceResource(ownerDevice), depthStencilViewType(depthStencilViewType) {

}

CLCGHAL_API DepthStencilView::~DepthStencilView() {}
