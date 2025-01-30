#include "control.h"

using namespace clench;
using namespace clench::vwc;

CLCVWC_API Control::Control(
	peff::Alloc *selfAllocator,
	wsal::CreateWindowFlags createWindowFlags,
	acri::Device *acriDevice,
	acri::DeviceContext *acriDeviceContext,
	ghal::TextureFormat renderBufferFormat,
	wsal::Window *parent,
	int x,
	int y,
	int width,
	int height)
	: VirtualWindow(selfAllocator, createWindowFlags, parent, x, y, width, height),
	  acriDevice(acriDevice),
	  acriDeviceContext(acriDeviceContext),
	  renderBufferFormat(renderBufferFormat) {
}

CLCVWC_API Control::~Control() {
}

CLCVWC_API void Control::onResize(int width, int height) {
	/* renderTargetView.reset();
	renderBuffer.reset();

	acri::Texture2DDesc textureDesc = {};

	textureDesc.format = renderBufferFormat;
	textureDesc.width = width;
	textureDesc.height = height;
	textureDesc.mipmapLevel = 0;
	textureDesc.bindingFlags =
		acri::TEXTURE_BINDING_RENDER_TARGET |
		acri::TEXTURE_BINDING_SHADER_RESOURCE;

	renderBuffer = acriDevice->createTexture2D(nullptr, 0, textureDesc);
	renderTargetView = acriDevice->createRenderTargetViewForTexture2D(renderBuffer.get());*/
}

CLCVWC_API void Control::onExpose() {
	redrawChildWindows();
}

CLCVWC_API const wsal::LayoutAttributes* Control::getLayoutAttributes() const {
	return layoutAttributes.get();
}

CLCVWC_API void Control::redrawChildWindows() {
	int viewportX, viewportY, viewportWidth, viewportHeight;
	float viewportMinDepth, viewportMaxDepth;

	int subwindowX, subwindowY;
	int subwindowWidth, subwindowHeight;

	for (auto i : _childWindows) {
		i->getPos(subwindowX, subwindowY);
		i->getSize(subwindowWidth, subwindowHeight);

		acriDeviceContext->ghalDeviceContext->getViewport(
			viewportX,
			viewportY,
			viewportWidth,
			viewportHeight,
			viewportMinDepth,
			viewportMaxDepth);

		acriDeviceContext->ghalDeviceContext->setViewport(
			viewportX + subwindowX,
			viewportY + subwindowY,
			subwindowWidth,
			subwindowHeight,
			0.0f,
			1.0f);

		i->onExpose();
	}
}
