#include "views.h"

using namespace clench;
using namespace clench::ghal;

CLCGHAL_API RenderTargetView::RenderTargetView(
	GHALDevice *ownerDevice,
	RenderTargetViewType renderTargetViewType)
	: GHALDeviceResource(ownerDevice), renderTargetViewType(renderTargetViewType) {
}

CLCGHAL_API RenderTargetView::~RenderTargetView() {}

CLCGHAL_API DepthStencilView::DepthStencilView(
	GHALDevice* ownerDevice,
	DepthStencilViewType depthStencilViewType
) : GHALDeviceResource(ownerDevice), depthStencilViewType(depthStencilViewType) {

}

CLCGHAL_API DepthStencilView::~DepthStencilView() {}
