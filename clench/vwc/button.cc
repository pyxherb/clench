#include "button.h"

using namespace clench;
using namespace clench::vwc;

CLCVWC_API Button::Button(
	peff::Alloc *selfAllocator,
	ghal::GHALDevice *ghalDevice,
	ghal::GHALDeviceContext *ghalDeviceContext,
	ghal::TextureFormat renderBufferFormat,
	Window *parent,
	int x,
	int y,
	int width,
	int height)
	: Control(
		  selfAllocator,
		  wsal::CREATEWINDOW_NOFRAME,
		  ghalDevice,
		  ghalDeviceContext,
		  renderBufferFormat,
		  parent,
		  x,
		  y,
		  width,
		  height) {
}

CLCVWC_API Button::~Button() {
}

CLCVWC_API void Button::dealloc() {
	peff::destroyAndRelease<Button>(selfAllocator.get(), this, sizeof(std::max_align_t));
}

CLCVWC_API void Button::onMouseButtonPress(wsal::MouseButton button, int x, int y) {
	if (button == wsal::MouseButton::Left) {
		onPress();
	}
}

CLCVWC_API void Button::onMouseButtonRelease(wsal::MouseButton button, int x, int y) {
	if (button == wsal::MouseButton::Left)
		onRelease();
}

CLCVWC_API void Button::onMouseHover(int x, int y) {
	onHover();
}

CLCVWC_API void Button::onMouseLeave() {
	onLeave();
}

CLCVWC_API DefaultButton::DefaultButton(
	peff::Alloc *selfAllocator,
	ghal::GHALDevice *ghalDevice,
	ghal::GHALDeviceContext *ghalDeviceContext,
	ghal::TextureFormat renderBufferFormat,
	Window *parent,
	int x,
	int y,
	int width,
	int height)
	: Button(
		  selfAllocator,
		  ghalDevice,
		  ghalDeviceContext,
		  renderBufferFormat,
		  parent,
		  x,
		  y,
		  width,
		  height) {
}

CLCVWC_API DefaultButton::~DefaultButton() {
}

CLCVWC_API void DefaultButton::dealloc() {
	peff::destroyAndRelease<DefaultButton>(selfAllocator.get(), this, sizeof(std::max_align_t));
}

CLCVWC_API void DefaultButton::onDraw() {
	ghalDeviceContext->setViewport(_x, _y, _width, _height, 0.0f, 1.0f);
	if (_isPressed) {
		ghalDeviceContext->clearRenderTargetView(
			nullptr,
			pressedBackgroundColor[0],
			pressedBackgroundColor[1],
			pressedBackgroundColor[2],
			1.0f);
	} else {
		if (_isHovered) {
			ghalDeviceContext->clearRenderTargetView(
				nullptr,
				hoverBackgroundColor[0],
				hoverBackgroundColor[1],
				hoverBackgroundColor[2],
				1.0f);
		} else {
			ghalDeviceContext->clearRenderTargetView(
				nullptr,
				backgroundColor[0],
				backgroundColor[1],
				backgroundColor[2],
				1.0f);
		}
	}
}

CLCVWC_API void DefaultButton::onHover() {
	if (!_isHovered) {
		_isHovered = true;
		invalidate();
	}
}

CLCVWC_API void DefaultButton::onLeave() {
	if (_isHovered) {
		_isHovered = false;
		invalidate();
	}
}

CLCVWC_API void DefaultButton::onPress() {
	if (!_isPressed) {
		_isPressed = true;
		wsal::setMouseCapture(this);
		invalidate();
	}
}

CLCVWC_API void DefaultButton::onRelease() {
	if (_isPressed) {
		_isPressed = false;
		invalidate();
		wsal::releaseMouseCapture();
	}
}

CLCVWC_API bool DefaultButton::isPressed() const {
	return _isPressed;
}
