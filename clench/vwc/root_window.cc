#include "root_window.h"

using namespace clench;
using namespace clench::vwc;

CLCVWC_API VWCRootWindow::VWCRootWindow(
	peff::Alloc *selfAllocator,
	wsal::Window *parent,
	ghal::Device *ghalDevice)
	: VirtualWindow(
		  selfAllocator,
		  0,
		  parent,
		  0,
		  0,
		  0,
		  0),
	  ghalDevice(ghalDevice) {
	ghalDevice->createDeviceContextForWindow(parent, ghalDeviceContext.getRef());

	int width, height;
	parent->getSize(width, height);

	setSize(width, height);
}

CLCVWC_API VWCRootWindow::~VWCRootWindow() {
}

CLCVWC_API void VWCRootWindow::dealloc() {
	peff::destroyAndRelease<VWCRootWindow>(selfAllocator.get(), this, sizeof(std::max_align_t));
}

CLCVWC_API void VWCRootWindow::onResize(int width, int height) {
	ghalDeviceContext->onResize(width, height);

	VirtualWindow::onResize(width, height);
}

CLCVWC_API void VWCRootWindow::onMove(int x, int y) {
	VirtualWindow::onMove(x, y);
}

CLCVWC_API bool VWCRootWindow::onClose() {
	return false;
}

CLCVWC_API void VWCRootWindow::onKeyDown(wsal::KeyboardKeyCode keyCode) {
}

CLCVWC_API void VWCRootWindow::onKeyUp(wsal::KeyboardKeyCode keyCode) {
}

CLCVWC_API void VWCRootWindow::onMouseButtonPress(wsal::MouseButton button, int x, int y) {
	VirtualWindow::onMouseButtonPress(button, x, y);
}

CLCVWC_API void VWCRootWindow::onMouseButtonRelease(wsal::MouseButton button, int x, int y) {
	VirtualWindow::onMouseButtonRelease(button, x, y);
}

CLCVWC_API void VWCRootWindow::onMouseHover(int x, int y) {
	VirtualWindow::onMouseHover(x, y);
}

CLCVWC_API void VWCRootWindow::onMouseLeave() {
	VirtualWindow::onMouseLeave();
}

CLCVWC_API void VWCRootWindow::onMouseMove(int x, int y) {
	VirtualWindow::onMouseMove(x, y);
}

CLCVWC_API void VWCRootWindow::onDraw() {
	ghalDeviceContext->begin();

	ghalDeviceContext->clearRenderTargetView(nullptr, 0.0f, 0.0f, 0.0f, 1.0f);

	int width, height;
	getSize(width, height);

	ghalDeviceContext->setViewport(0, 0, width, height, 0.0f, 1.0f);

	for (auto i : _childWindows) {
		int subwindowX, subwindowY;
		int subwindowWidth, subwindowHeight;

		i->getPos(subwindowX, subwindowY);
		i->getSize(subwindowWidth, subwindowHeight);

		ghalDeviceContext->setViewport(
			subwindowX,
			subwindowY,
			subwindowWidth,
			subwindowHeight,
			0.0f,
			1.0f);

		i->onDraw();
	}

	ghalDeviceContext->end();
	ghalDeviceContext->present();
}

CLCVWC_API const wsal::LayoutAttributes *VWCRootWindow::getLayoutAttributes() const {
	return &layoutAttribs;
}
