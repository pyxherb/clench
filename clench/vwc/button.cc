#include "button.h"

using namespace clench;
using namespace clench::vwc;

CLCVWC_API Button::Button(
	peff::Alloc *selfAllocator,
	acri::Device *acriDevice,
	acri::DeviceContext *acriDeviceContext,
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
	Window *parent,
	int x,
	int y,
	int width,
	int height)
	: Button(
		  selfAllocator,
		  acriDevice,
		  acriDeviceContext,
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
	acri::RectParams rectParams;

	rectParams.left = -1.0f;
	rectParams.right = 1.0f;
	rectParams.top = 1.0f;
	rectParams.bottom = -1.0f;

	if (_isPressed) {
		acriDeviceContext->fillRect(rectParams, pressedBackgroundBrush.get());
	} else {
		if (_isHovered) {
			acriDeviceContext->fillRect(rectParams, hoverBackgroundBrush.get());
		} else {
			acriDeviceContext->fillRect(rectParams, backgroundBrush.get());
		}
	}

	math::Vec2f vertices[] = {
		{ 3.0f, 48.0f },
		{ 52.0f, 8.0f },
		{ 99.0f, 50.0f },
		{ 138.0f, 25.0f },
		{ 175.0f, 77.0f },
		{ 131.0f, 72.0f },
		{ 111.0f, 113.0f },
		{ 72.0f, 43.0f },
		{ 26.0f, 55.0f },
		{ 29.0f, 100.0f }
	};

	acri::PolygonParams polygonParams;
	polygonParams.vertices = vertices;
	polygonParams.nVertices = 10;

	acriDeviceContext->fillPolygon(polygonParams, pressedBackgroundBrush.get());

	acri::EllipseParams ellipseParams;
	ellipseParams.origin = { 100.0f, 100.0f };
	ellipseParams.radiusX = 70.0f;
	ellipseParams.radiusY = 70.0f;

	acriDeviceContext->fillEllipse(ellipseParams, pressedBackgroundBrush.get());
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

CLCVWC_API DefaultButton *DefaultButton::alloc(
	peff::Alloc *selfAllocator,
	acri::Device *acriDevice,
	acri::DeviceContext *acriDeviceContext,
	Window *parent,
	int x,
	int y,
	int width,
	int height) {
	std::unique_ptr<DefaultButton, peff::RcObjectUniquePtrDeleter> ptr(
		peff::allocAndConstruct<clench::vwc::DefaultButton>(
			selfAllocator,
			sizeof(std::max_align_t),
			selfAllocator,
			acriDevice,
			acriDeviceContext,
			parent,
			x,
			y,
			width,
			height));

	if (!(ptr->backgroundBrush = acriDevice->createSolidColorBrush(backgroundColor))) {
		return nullptr;
	}

	if (!(ptr->hoverBackgroundBrush = acriDevice->createSolidColorBrush(hoverBackgroundColor))) {
		return nullptr;
	}

	if (!(ptr->pressedBackgroundBrush = acriDevice->createSolidColorBrush(pressedBackgroundColor))) {
		return nullptr;
	}

	return ptr.release();
}
