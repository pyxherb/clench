#include "control.h"

using namespace clench;
using namespace clench::vwc;

CLCVWC_API Control::Control(
	peff::Alloc *selfAllocator,
	wsal::CreateWindowFlags createWindowFlags,
	ghal::Device *ghalDevice,
	ghal::DeviceContext *ghalDeviceContext,
	ghal::TextureFormat renderBufferFormat,
	wsal::Window *parent,
	int x,
	int y,
	int width,
	int height)
	: VirtualWindow(selfAllocator, createWindowFlags, parent, x, y, width, height),
	  ghalDevice(ghalDevice),
	  ghalDeviceContext(ghalDeviceContext),
	  renderBufferFormat(renderBufferFormat) {
}

CLCVWC_API Control::~Control() {
}

CLCVWC_API void Control::onResize(int width, int height) {
	/* renderTargetView.reset();
	renderBuffer.reset();

	ghal::Texture2DDesc textureDesc = {};

	textureDesc.format = renderBufferFormat;
	textureDesc.width = width;
	textureDesc.height = height;
	textureDesc.mipmapLevel = 0;
	textureDesc.bindingFlags =
		ghal::TEXTURE_BINDING_RENDER_TARGET |
		ghal::TEXTURE_BINDING_SHADER_RESOURCE;

	renderBuffer = ghalDevice->createTexture2D(nullptr, 0, textureDesc);
	renderTargetView = ghalDevice->createRenderTargetViewForTexture2D(renderBuffer.get());*/
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

		ghalDeviceContext->getViewport(
			viewportX,
			viewportY,
			viewportWidth,
			viewportHeight,
			viewportMinDepth,
			viewportMaxDepth);

		ghalDeviceContext->setViewport(
			viewportX + subwindowX,
			viewportY + subwindowY,
			subwindowWidth,
			subwindowHeight,
			0.0f,
			1.0f);

		i->onExpose();
	}
}
