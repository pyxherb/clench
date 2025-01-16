#include "window.h"

// Fuck <Windows.h>, we have to undefine them to use std::min and std::max.
#ifdef _WIN32
	#undef min
	#undef max
#endif

using namespace clench;
using namespace clench::wsal;

CLCWSAL_API WindowScope::WindowScope(peff::Alloc *selfAllocator, peff::Alloc *allocator) : selfAllocator(selfAllocator), allocator(allocator) {
}

CLCWSAL_API WindowScope::~WindowScope() {
	assert((!childWindows.size(), "Not all child windows are released"));
}

CLCWSAL_API void WindowScope::dealloc() {
	peff::destroyAndRelease<WindowScope>(selfAllocator.get(), this, sizeof(std::max_align_t));
}

CLCWSAL_API WindowScope *WindowScope::alloc(peff::Alloc *selfAllocator, peff::Alloc *allocator) {
	return peff::allocAndConstruct<WindowScope>(
		selfAllocator, sizeof(std::max_align_t),
		selfAllocator, allocator);
}

CLCWSAL_API wsal::Window::Window(WindowScope *windowScope) : windowScope(windowScope) {}

CLCWSAL_API wsal::Window::~Window() {
}

void wsal::Window::onRefZero() noexcept {
	peff::destroyAndRelease<Window>(windowScope->allocator.get(), this, sizeof(std::max_align_t));
}

CLCWSAL_API VirtualWindow::VirtualWindow(
	WindowScope *windowScope,
	CreateWindowFlags flags,
	Window *parent,
	int x,
	int y,
	int width,
	int height)
	: Window(windowScope),
	  _createWindowFlags(flags),
	  _parent(parent),
	  _x(x),
	  _y(y),
	  _width(width),
	  _height(height) {
	if (parent)
		setParent(parent);
}

CLCWSAL_API VirtualWindow::~VirtualWindow() {
}

CLCWSAL_API void VirtualWindow::show() {
	_shown = true;
}

CLCWSAL_API void VirtualWindow::hide() {
	_shown = false;
}

CLCWSAL_API bool VirtualWindow::isVisible() const {
	return _shown;
}

CLCWSAL_API bool VirtualWindow::isClosed() const {
	return _closed;
}

CLCWSAL_API void VirtualWindow::setPos(int x, int y) {
	_x = x;
	_y = y;
}

CLCWSAL_API void VirtualWindow::getPos(int &xOut, int &yOut) const {
	xOut = _x;
	yOut = _y;
}

CLCWSAL_API void VirtualWindow::setSize(int width, int height) {
	_width = width;
	_height = height;
}

CLCWSAL_API void VirtualWindow::getSize(int &widthOut, int &heightOut) const {
	widthOut = _width;
	heightOut = _height;
}

CLCWSAL_API void VirtualWindow::setTitle(const char *title) {
	_title = title;
}

CLCWSAL_API void VirtualWindow::setParent(Window *window) {
	window->addChildWindow(this);
	_parent = window;
}

CLCWSAL_API wsal::Window *VirtualWindow::getParent() const {
	return _parent;
}

CLCWSAL_API void VirtualWindow::addChildWindow(Window *window) {
	{
		WindowProperties windowProperties;
		window->getWindowProperties(windowProperties);
		if (!windowProperties.isNative)
			throw std::logic_error("Cannot add a native window onto a virtual window");
	}
	_childWindows.insert((VirtualWindow *)window);
}

CLCWSAL_API void VirtualWindow::removeChildWindow(Window *window) {
	_childWindows.erase((VirtualWindow *)window);
}

CLCWSAL_API bool VirtualWindow::hasChildWindow(Window *window) const {
	return _childWindows.count((VirtualWindow *)window);
}

CLCWSAL_API void VirtualWindow::enumChildWindows(ChildWindowEnumer &&enumer) {
	for (auto i : _childWindows) {
		if (!enumer(i.get()))
			break;
	}
}

CLCWSAL_API void VirtualWindow::getWindowProperties(WindowProperties &propertiesOut) const {
	propertiesOut = {};

	propertiesOut.isNative = false;
}

CLCWSAL_API void VirtualWindow::pollEvents() {
}

CLCWSAL_API void VirtualWindow::invalidate() {
	_parent->invalidate();
}

CLCWSAL_API void VirtualWindow::onResize(int width, int height) {
	_width = width;
	_height = height;
}

CLCWSAL_API void VirtualWindow::onMove(int x, int y) {
	_x = x;
	_y = y;
}

CLCWSAL_API bool VirtualWindow::onClose() {
	return false;
}

CLCWSAL_API void VirtualWindow::onKeyDown(KeyboardKeyCode keyCode) {
}

CLCWSAL_API void VirtualWindow::onKeyUp(KeyboardKeyCode keyCode) {
}

CLCWSAL_API void VirtualWindow::onMouseButtonPress(MouseButton button, int x, int y) {
}

CLCWSAL_API void VirtualWindow::onMouseButtonRelease(MouseButton button, int x, int y) {
}

CLCWSAL_API void VirtualWindow::onMouseHover(int x, int y) {
}

CLCWSAL_API void VirtualWindow::onMouseLeave() {
}

CLCWSAL_API void VirtualWindow::onMouseMove(int x, int y) {
}

CLCWSAL_API void VirtualWindow::onExpose() {
	onDraw();
}

CLCWSAL_API void VirtualWindow::onDraw() {
	for (auto i : _childWindows)
		i->onDraw();
}

CLCWSAL_API const LayoutAttributes *VirtualWindow::getLayoutAttributes() const {
	return nullptr;
}

CLCWSAL_API wsal::Window *wsal::getRootNativeWindow(Window *childWindow) {
	Window *i = childWindow;
	WindowProperties windowProperties;

	while (i) {
		i->getWindowProperties(windowProperties);
		if (windowProperties.isNative)
			return i;
		i = i->getParent();
	}

	return nullptr;
}

CLCWSAL_API void wsal::getAbsoluteOffsetToRootNativeWindow(Window *window, int &xOut, int &yOut) {
	xOut = 0;
	yOut = 0;

	WindowProperties windowProperties;

	while (true) {
		window->getWindowProperties(windowProperties);

		if (windowProperties.isNative)
			return;

		int relativeX, relativeY;

		window->getPos(relativeX, relativeY);

		xOut += relativeX, yOut += relativeY;

		window = window->getParent();
	}
}

CLCWSAL_API void wsal::calcWindowLayout(
	const LayoutAttributes *layoutAttributes,
	int parentWindowX,
	int parentWindowY,
	int parentWindowWidth,
	int parentWindowHeight,
	int windowX,
	int windowY,
	int windowWidth,
	int windowHeight,
	int &windowXOut,
	int &windowYOut,
	int &windowWidthOut,
	int &windowHeightOut) {
	int x = 0, y = 0, width = 0, height = 0;

	switch (layoutAttributes->horizontalAlignment) {
		case HorizontalAlignment::Left:
			x = 0;
			width = windowWidth;

			if (layoutAttributes->marginBox.left != UNSET) {
				x += layoutAttributes->marginBox.left;
			}
			break;
		case HorizontalAlignment::Center:
			x = parentWindowWidth / 2 - windowWidth / 2;
			width = windowWidth;

			if (layoutAttributes->marginBox.left != UNSET) {
				x += layoutAttributes->marginBox.left;
			}

			if (layoutAttributes->marginBox.right != UNSET) {
				x -= layoutAttributes->marginBox.left;
			}
			break;
		case HorizontalAlignment::Right:
			x = parentWindowWidth - windowWidth;
			width = windowWidth;

			if (layoutAttributes->marginBox.right != UNSET) {
				x -= layoutAttributes->marginBox.right;
			}
			break;
		case HorizontalAlignment::Stretch:
			if (layoutAttributes->positioningBox.left == UNSET) {
				x = 0;
			} else {
				x = layoutAttributes->positioningBox.left;
			}

			if (layoutAttributes->marginBox.left != UNSET)
				x += layoutAttributes->marginBox.left;

			if (layoutAttributes->positioningBox.right == UNSET) {
				width = parentWindowWidth - x;
			} else {
				width =
					parentWindowWidth -
					layoutAttributes->positioningBox.right;
			}

			if (layoutAttributes->marginBox.right != UNSET)
				width -= layoutAttributes->marginBox.right;
			break;
		default:
			throw std::logic_error("Invalid layout attributes combination");
	}
	switch (layoutAttributes->verticalAlignment) {
		case VerticalAlignment::Top:
			y = 0;
			height = windowHeight;

			if (layoutAttributes->marginBox.top != UNSET) {
				y += layoutAttributes->marginBox.top;
			}
			break;
		case VerticalAlignment::Center:
			y = parentWindowHeight / 2 - windowHeight / 2;
			height = windowHeight;

			if (layoutAttributes->marginBox.top != UNSET) {
				x += layoutAttributes->marginBox.left;
			}

			if (layoutAttributes->marginBox.bottom != UNSET) {
				x -= layoutAttributes->marginBox.left;
			}
			break;
		case VerticalAlignment::Bottom:
			y = parentWindowHeight - windowHeight;
			height = windowHeight;

			if (layoutAttributes->marginBox.bottom != UNSET) {
				y -= layoutAttributes->marginBox.bottom;
			}
			break;
		case VerticalAlignment::Stretch:
			if (layoutAttributes->positioningBox.top == UNSET) {
				y = 0;
			} else {
				y = layoutAttributes->positioningBox.top;
			}

			if (layoutAttributes->marginBox.top != UNSET)
				y += layoutAttributes->marginBox.top;

			if (layoutAttributes->positioningBox.bottom == UNSET) {
				height = parentWindowHeight - y;
			} else {
				height =
					parentWindowHeight -
					layoutAttributes->positioningBox.bottom;
			}

			if (layoutAttributes->marginBox.bottom != UNSET)
				height -= layoutAttributes->marginBox.bottom;
			break;
		default:
			throw std::logic_error("Invalid layout attributes combination");
	}

	windowXOut = x;
	windowYOut = y;
	windowWidthOut = width;
	windowHeightOut = height;
}
