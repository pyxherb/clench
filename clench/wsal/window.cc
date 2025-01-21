#include "window.h"
#include "backend.h"

// Fuck <Windows.h>, we have to undefine them to use std::min and std::max.
#ifdef _WIN32
	#undef min
	#undef max
#endif

using namespace clench;
using namespace clench::wsal;

CLCWSAL_API wsal::Window::Window(Backend *backend) : backend(backend) {}

CLCWSAL_API wsal::Window::~Window() {
}

CLCWSAL_API void wsal::Window::onRefZero() noexcept {
	peff::destroyAndRelease<Window>(backend->resourceAllocator.get(), this, sizeof(std::max_align_t));
}

CLCWSAL_API VirtualWindow::VirtualWindow(
	peff::Alloc *selfAllocator,
	CreateWindowFlags flags,
	Window *parent,
	int x,
	int y,
	int width,
	int height)
	: Window(nullptr),
	  selfAllocator(selfAllocator),
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

CLCWSAL_API void VirtualWindow::onRefZero() noexcept {
	peff::destroyAndRelease<Window>(selfAllocator.get(), this, sizeof(std::max_align_t));
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

	for (auto i = _childWindows.begin(); i != _childWindows.end(); ++i) {
		if ((*i)->backend)
			continue;

		if (const wsal::LayoutAttributes *layoutAttribs =
				((wsal::VirtualWindow *)(*i).get())->getLayoutAttributes();
			layoutAttribs) {
			int windowX, windowY, windowWidth, windowHeight;
			int newX, newY, newWidth, newHeight;

			(*i)->getPos(windowX, windowY);
			(*i)->getSize(windowWidth, windowHeight);

			wsal::calcWindowLayout(
				layoutAttribs,
				0, 0,
				_width, _height,
				windowX, windowY,
				windowWidth, windowHeight,
				newX, newY,
				newWidth, newHeight);

			(*i)->setPos(newX, newY);
			(*i)->setSize(newWidth, newHeight);
		}
	}
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
	if (window->backend)
		throw std::logic_error("Cannot add a native window onto a virtual window");
	assert(("Cannot add a virtual window onto itself", this != window));
	_childWindows.insert((VirtualWindow *)window);
}

CLCWSAL_API void VirtualWindow::removeChildWindow(Window *window) {
	_childWindows.remove((VirtualWindow *)window);
}

CLCWSAL_API bool VirtualWindow::hasChildWindow(Window *window) const {
	return _childWindows.contains((VirtualWindow *)window);
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

	wsal::WindowProperties windowProperties;

	for (auto i : _childWindows) {
		i->getWindowProperties(windowProperties);

		if (windowProperties.isNative)
			continue;

		if (const wsal::LayoutAttributes *layoutAttribs =
				((wsal::VirtualWindow *)i.get())->getLayoutAttributes();
			layoutAttribs) {
			int windowX, windowY, windowWidth, windowHeight;
			int newX, newY, newWidth, newHeight;

			i->getPos(windowX, windowY);
			i->getSize(windowWidth, windowHeight);

			wsal::calcWindowLayout(
				layoutAttribs,
				0, 0,
				width, height,
				windowX, windowY,
				windowWidth, windowHeight,
				newX, newY,
				newWidth, newHeight);

			i->setPos(newX, newY);
			i->setSize(newWidth, newHeight);
		}
	}
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
	peff::Map<Window *, std::pair<int, int>> childWindows;

	findWindowsAtPos(x, y, childWindows);

	for (auto i = childWindows.begin(); i != childWindows.end(); ++i) {
		i.key()->onMouseButtonPress(button, i.value().first, i.value().second);
	}
}

CLCWSAL_API void VirtualWindow::onMouseButtonRelease(MouseButton button, int x, int y) {
	peff::Map<Window *, std::pair<int, int>> childWindows;

	findWindowsAtPos(x, y, childWindows);

	for (auto i = childWindows.begin(); i != childWindows.end(); ++i) {
		i.key()->onMouseButtonRelease(button, i.value().first, i.value().second);
	}
}

CLCWSAL_API void VirtualWindow::onMouseHover(int x, int y) {
	peff::Map<Window *, std::pair<int, int>> childWindows;

	findWindowsAtPos(x, y, childWindows);

	for (auto i = childWindows.begin(); i != childWindows.end(); ++i) {
		i.key()->onMouseHover(i.value().first, i.value().second);
	}
}

CLCWSAL_API void VirtualWindow::onMouseLeave() {
	/*
	for (auto i : hoveredChildWindows) {
		i->onMouseLeave();
	}

	hoveredChildWindows.clear();*/
}

CLCWSAL_API void VirtualWindow::onMouseMove(int x, int y) {
	peff::Map<Window *, std::pair<int, int>> childWindows;

	findWindowsAtPos(x, y, childWindows);

	{
		peff::Set<Window *> leftWindows;
		for (auto i : hoveredChildWindows) {
			if (!childWindows.contains(i)) {
				auto copiedI = i;
				leftWindows.insert(std::move(copiedI));
			}
		}

		for (auto i : leftWindows) {
			auto copiedI = i;
			i->onMouseLeave();
			hoveredChildWindows.remove(std::move(copiedI));
		}
	}

	for (auto i = childWindows.begin(); i != childWindows.end(); ++i) {
		if (!hoveredChildWindows.contains(i.key())) {
			i.key()->onMouseHover(x, y);
			Window *copiedWindow = i.key();
			hoveredChildWindows.insert(std::move(copiedWindow));
		} else
			i.key()->onMouseMove(i.value().first, i.value().second);
	}
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
		if (window->backend)
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

CLCWSAL_API void VirtualWindow::redrawChildWindows() {
	for (auto i : _childWindows) {
		i->onExpose();
	}
}

CLCWSAL_API void VirtualWindow::findWindowsAtPos(int x, int y, peff::Map<Window *, std::pair<int, int>> &childWindowsOut) {
	for (auto i : _childWindows) {
		int windowX, windowY, windowWidth, windowHeight;
		i->getPos(windowX, windowY);
		i->getSize(windowWidth, windowHeight);

		if ((x >= windowX) &&
			(y >= windowY) &&
			(x < windowX + windowWidth) &&
			(y < windowY + windowHeight))
			childWindowsOut.insert(i.get(), { x - windowX, y - windowY });
	}
}
