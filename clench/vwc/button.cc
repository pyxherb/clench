#include "button.h"

using namespace clench;
using namespace clench::vwc;

CLCVWC_API Button::Button(
	peff::Alloc *selfAllocator,
	acri::Device *acriDevice,
	acri::DeviceContext *acriDeviceContext,
	ghal::TextureFormat renderBufferFormat,
	Window *parent,
	int x,
	int y,
	int width,
	int height)
	: Control(
		  selfAllocator,
		  wsal::CREATEWINDOW_NOFRAME,
		  acriDevice,
		  acriDeviceContext,
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
	acri::Device *acriDevice,
	acri::DeviceContext *acriDeviceContext,
	ghal::TextureFormat renderBufferFormat,
	Window *parent,
	int x,
	int y,
	int width,
	int height)
	: Button(
		  selfAllocator,
		  acriDevice,
		  acriDeviceContext,
		  renderBufferFormat,
		  parent,
		  x,
		  y,
		  width,
		  height) {
	backgroundBrush = acriDevice->createSolidColorBrush(backgroundColor);
	hoverBackgroundBrush = acriDevice->createSolidColorBrush(hoverBackgroundColor);
	pressedBackgroundBrush = acriDevice->createSolidColorBrush(pressedBackgroundColor);
}

CLCVWC_API DefaultButton::~DefaultButton() {
}

CLCVWC_API void DefaultButton::dealloc() {
	peff::destroyAndRelease<DefaultButton>(selfAllocator.get(), this, sizeof(std::max_align_t));
}

CLCVWC_API void DefaultButton::onDraw() {
	acri::TriangleParams triangleParams;

	triangleParams.vertices[0].x = -1.0f;
	triangleParams.vertices[0].y = -1.0f;

	triangleParams.vertices[1].x = 1.0f;
	triangleParams.vertices[1].y = -1.0f;

	triangleParams.vertices[2].x = -1.0f;
	triangleParams.vertices[2].y = 1.0f;

	if (_isPressed) {
		acriDeviceContext->fillTriangle(triangleParams, pressedBackgroundBrush.get());
	} else {
		if (_isHovered) {
			acriDeviceContext->fillTriangle(triangleParams, hoverBackgroundBrush.get());
		} else {
			acriDeviceContext->fillTriangle(triangleParams, backgroundBrush.get());
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
