#include "window.h"
#include "backend.h"
#include <clench/wsal/window.h>
#include <clench/utils/logger.h>

#include <locale>
#include <map>
#include <stdexcept>

using namespace clench;
using namespace clench::wsal;

CLCWSAL_API X11Window::X11Window(
	X11Backend *backend,
	X11WindowHandle nativeHandle) : Window(backend), nativeHandle(nativeHandle) {
}

CLCWSAL_API X11Window::~X11Window() {
	X11Backend *backend = ((X11Backend *)this->backend);
	if (auto it = backend->handleToWindowMap.find(nativeHandle); it != backend->handleToWindowMap.end())
		backend->handleToWindowMap.remove(it);

	XDestroyWindow(nativeHandle.display, nativeHandle.window);
	XCloseDisplay(nativeHandle.display);
}

CLCWSAL_API void X11Window::dealloc() {
	peff::destroyAndRelease<X11Window>(backend->resourceAllocator.get(), this, sizeof(std::max_align_t));
}

CLCWSAL_API void X11Window::pollEvents() {
	X11Backend *backend = ((X11Backend *)this->backend);
	XEvent ev;

	while (XPending(nativeHandle.display) && (!_isClosed)) {
		XNextEvent(nativeHandle.display, &ev);

		XPointer window;
		XContext context;

		switch (ev.type) {
			case KeyPress: {
				auto key = XLookupKeysym(&ev.xkey, 0);

				if (!_keyPressedTimes.contains(key)) {
					if (!_keyPressedTimes.insert(+key, +ev.xkey.time))
						break;
				}

				Time timeDiff = ev.xkey.time - _keyPressedTimes.at(key);

				if ((timeDiff == ev.xkey.time) || ((timeDiff > 0) & (timeDiff < ((Time)0xffffffff)))) {
					_keyPressedTimes.at(key) = ev.xkey.time;

					if (auto it = backend->builtNativeKeyMap.find(key); it != backend->builtNativeKeyMap.end()) {
						onKeyDown((KeyboardKeyCode)it.value());
					}
				}
				break;
			}
			case KeyRelease: {
				auto key = XLookupKeysym(&ev.xkey, 0);

				if (XEventsQueued(nativeHandle.display, QueuedAfterReading)) {
					XEvent nextEvent;
					XPeekEvent(nativeHandle.display, &nextEvent);

					if (nextEvent.type == KeyPress &&
						nextEvent.xkey.window == ev.xkey.window &&
						nextEvent.xkey.keycode == ev.xkey.keycode) {
						if (nextEvent.xkey.time - ev.xkey.time < 20)
							continue;
					}
				}

				if (auto it = backend->builtNativeKeyMap.find(key); it != backend->builtNativeKeyMap.end()) {
					onKeyUp((KeyboardKeyCode)it.value());
				}

				_keyPressedTimes.remove(key);
				break;
			}
			case ButtonPress: {
				auto button = ev.xbutton.button;

				MouseButton mappedButton;

				switch (button) {
					case 1:
						mappedButton = MouseButton::Left;
						break;
					case 2:
						mappedButton = MouseButton::Middle;
						break;
					case 3:
						mappedButton = MouseButton::Right;
						break;
				}

				onMouseButtonPress(mappedButton, ev.xbutton.x, ev.xbutton.y);

				break;
			}
			case ButtonRelease: {
				auto button = ev.xbutton.button;

				MouseButton mappedButton;

				switch (button) {
					case 1:
						mappedButton = MouseButton::Left;
						break;
					case 2:
						mappedButton = MouseButton::Middle;
						break;
					case 3:
						mappedButton = MouseButton::Right;
						break;
				}

				onMouseButtonRelease(mappedButton, ev.xbutton.x, ev.xbutton.y);

				break;
			}
			case MotionNotify: {
				onMouseMove(ev.xbutton.x, ev.xbutton.y);
				break;
			}
			case EnterNotify:
				onMouseHover(ev.xcrossing.x, ev.xcrossing.y);
				break;
			case LeaveNotify:
				onMouseLeave();
				break;
			case ConfigureNotify: {
				/*if (ev.xconfigure.width != _width ||
					ev.xconfigure.height != _height)*/
				setSize(ev.xconfigure.width, ev.xconfigure.height);

				/*if (ev.xconfigure.x != _x || ev.xconfigure.y != _y)*/
				onMove(ev.xconfigure.x, ev.xconfigure.y);

				break;
			}
			case ResizeRequest:
				onResize(ev.xresizerequest.width, ev.xresizerequest.height);

				[[fallthrough]];
			case Expose:
			case GraphicsExpose:
				XSync(nativeHandle.display, false);
				break;
			case ClientMessage: {
				const Atom msg = ev.xclient.data.l[0];

				if (msg == XInternAtom(nativeHandle.display, "WM_DELETE_WINDOW", true)) {
					if (onClose())
						break;

					XUnmapWindow(nativeHandle.display, nativeHandle.window);
					this->_isClosed = true;
					break;
				} else if (msg == XInternAtom(nativeHandle.display, "NET_WM_PING", true)) {
					XEvent reply = ev;
					auto root = XRootWindow(nativeHandle.display, XDefaultScreen(nativeHandle.display));
					reply.xclient.window = root;

					XSendEvent(nativeHandle.display, root, False, SubstructureNotifyMask | SubstructureRedirectMask, &reply);
				}
				break;
			}
		}
	}
}

CLCWSAL_API void X11Window::show() {
	XMapWindow(nativeHandle.display, nativeHandle.window);
}

CLCWSAL_API void X11Window::hide() {
	XUnmapWindow(nativeHandle.display, nativeHandle.window);
}

CLCWSAL_API bool X11Window::isVisible() const {
	XWindowAttributes attribs;
	XGetWindowAttributes(nativeHandle.display, nativeHandle.window, &attribs);

	return attribs.map_state == IsViewable;
}

CLCWSAL_API bool X11Window::isClosed() const {
	return _isClosed;
}

CLCWSAL_API void X11Window::setPos(int x, int y) {
	XMoveWindow(nativeHandle.display, nativeHandle.window, x, y);
}

CLCWSAL_API void X11Window::getPos(int &xOut, int &yOut) const {
	XWindowAttributes attribs;

	XGetWindowAttributes(nativeHandle.display, nativeHandle.window, &attribs);

	xOut = attribs.x;
	yOut = attribs.y;
}

CLCWSAL_API void X11Window::setSize(int width, int height) {
	XResizeWindow(nativeHandle.display, nativeHandle.window, width, height);

	for (auto i = _childVirtualWindows.begin(); i != _childVirtualWindows.end(); ++i) {
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
				width, height,
				windowX, windowY,
				windowWidth, windowHeight,
				newX, newY,
				newWidth, newHeight);

			(*i)->setPos(newX, newY);
			(*i)->setSize(newWidth, newHeight);
		}
	}
}

CLCWSAL_API void X11Window::getSize(int &widthOut, int &heightOut) const {
	XWindowAttributes attribs;

	XGetWindowAttributes(nativeHandle.display, nativeHandle.window, &attribs);

	widthOut = attribs.width;
	heightOut = attribs.height;
}

CLCWSAL_API void X11Window::setTitle(const char *title) {
	XmbSetWMProperties(
		nativeHandle.display,
		nativeHandle.window,
		title, title,
		nullptr, 0,
		nullptr, nullptr, nullptr);
}

CLCWSAL_API void X11Window::setParent(Window *window) {
	WindowProperties windowProperties;
	window->getWindowProperties(windowProperties);

	if (!windowProperties.isNative)
		throw std::logic_error("Cannot set parent of a native window to a non-native window");

	XReparentWindow(
		((X11Window *)window)->nativeHandle.display,
		nativeHandle.window,
		((X11Window *)window)->nativeHandle.window,
		0,
		0);
}

CLCWSAL_API wsal::Window *X11Window::getParent() const {
	X11Backend *backend = ((X11Backend *)this->backend);
	::Window rootWindow, parentWindow, *children;
	unsigned int nChildren;

	if (XQueryTree(nativeHandle.display, nativeHandle.window, &rootWindow, &parentWindow, &children, &nChildren))
		return nullptr;

	XFree(children);

	if (auto it = backend->handleToWindowMap.find(X11WindowHandle{ parentWindow, nativeHandle.display });
		it != backend->handleToWindowMap.end())
		return it.value();

	return nullptr;
}

CLCWSAL_API base::ExceptionPtr X11Window::addChildWindow(wsal::Window *window) {
	if (!window->isNative()) {
		if (!_childVirtualWindows.insert((VirtualWindow *)window))
			return base::OutOfMemoryException::alloc();
	}
	return {};
}

CLCWSAL_API void X11Window::removeChildWindow(wsal::Window *window) {
	if (!window->isNative()) {
		X11Window *nativeWindow = ((X11Window *)window);

		if (nativeWindow->getParent() != this)
			return;

		::Window rootWindow, parentWindow, *children;
		unsigned int nChildren;

		if (XQueryTree(nativeHandle.display, nativeHandle.window, &rootWindow, &parentWindow, &children, &nChildren))
			return;

		XFree(children);

		XReparentWindow(nativeHandle.display, nativeWindow->nativeHandle.window, rootWindow, 0, 0);
	} else {
		_childVirtualWindows.remove((VirtualWindow *)window);
	}
}

CLCWSAL_API bool X11Window::hasChildWindow(wsal::Window *window) const {
	WindowProperties windowProperties;
	window->getWindowProperties(windowProperties);

	if (!windowProperties.isNative)
		return false;

	return ((X11Window *)window)->getParent() == this;
}

CLCWSAL_API void X11Window::enumChildWindows(ChildWindowEnumer &&enumer) {
	X11Backend *backend = ((X11Backend *)this->backend);
	::Window rootWindow, parentWindow, *children;
	unsigned int nChildren;

	if (XQueryTree(nativeHandle.display, nativeHandle.window, &rootWindow, &parentWindow, &children, &nChildren))
		return;

	peff::ScopeGuard freeChildrenGuard([children]() noexcept {
		XFree(children);
	});

	for (unsigned int i = 0; i < nChildren; ++i) {
		if (auto it = backend->handleToWindowMap.find(X11WindowHandle{ children[i], nativeHandle.display });
			it != backend->handleToWindowMap.end()) {
			enumer(it.value());
		}
	}

	for (auto i = _childVirtualWindows.begin(); i != _childVirtualWindows.end(); ++i) {
		enumer((*i).get());
	}
}

CLCWSAL_API void X11Window::getWindowProperties(WindowProperties &propertiesOut) const {
	propertiesOut = {};
	propertiesOut.isNative = true;
}

CLCWSAL_API void X11Window::invalidate() {
	onExpose();
}

CLCWSAL_API void X11Window::onResize(int width, int height) {
	for (auto i : _childVirtualWindows) {
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

CLCWSAL_API void X11Window::onMove(int x, int y) {
}

CLCWSAL_API bool X11Window::onClose() {
	return false;
}

CLCWSAL_API void X11Window::onKeyDown(KeyboardKeyCode keyCode) {
}

CLCWSAL_API void X11Window::onKeyUp(KeyboardKeyCode keyCode) {
}

CLCWSAL_API void X11Window::onMouseButtonPress(MouseButton button, int x, int y) {
	if (auto capturedWindow = wsal::getMouseCapture(); capturedWindow) {
		int xOffset, yOffset;
		wsal::getAbsoluteOffsetToRootNativeWindow(capturedWindow, xOffset, yOffset);

		capturedWindow->onMouseButtonPress(button, x - xOffset, y - yOffset);
	} else {
		peff::Map<VirtualWindow *, std::pair<int, int>> childWindows;

		findWindowsAtPos(x, y, childWindows);

		for (auto i = childWindows.begin(); i != childWindows.end(); ++i) {
			i.key()->onMouseButtonPress(button, i.value().first, i.value().second);
		}
	}
}

CLCWSAL_API void X11Window::onMouseButtonRelease(MouseButton button, int x, int y) {
	if (auto capturedWindow = wsal::getMouseCapture(); capturedWindow) {
		int xOffset, yOffset;
		wsal::getAbsoluteOffsetToRootNativeWindow(capturedWindow, xOffset, yOffset);

		capturedWindow->onMouseButtonRelease(button, x - xOffset, y - yOffset);
	} else {
		peff::Map<VirtualWindow *, std::pair<int, int>> childWindows;

		findWindowsAtPos(x, y, childWindows);

		for (auto i = childWindows.begin(); i != childWindows.end(); ++i) {
			i.key()->onMouseButtonRelease(button, i.value().first, i.value().second);
		}
	}
}

CLCWSAL_API void X11Window::onMouseHover(int x, int y) {
	if (auto capturedWindow = wsal::getMouseCapture(); capturedWindow) {
		int xOffset, yOffset;
		wsal::getAbsoluteOffsetToRootNativeWindow(capturedWindow, xOffset, yOffset);

		capturedWindow->onMouseHover(x - xOffset, y - yOffset);
	} else {
		peff::Map<VirtualWindow *, std::pair<int, int>> childWindows;

		findWindowsAtPos(x, y, childWindows);

		for (auto i = childWindows.begin(); i != childWindows.end(); ++i) {
			i.key()->onMouseHover(i.value().first, i.value().second);
		}
	}
}

CLCWSAL_API void X11Window::onMouseLeave() {
}

CLCWSAL_API void X11Window::onMouseMove(int x, int y) {
	if (auto capturedWindow = wsal::getMouseCapture(); capturedWindow) {
		int xOffset, yOffset;
		wsal::getAbsoluteOffsetToRootNativeWindow(capturedWindow, xOffset, yOffset);

		int width, height;

		capturedWindow->getSize(width, height);

		if ((x >= xOffset) &&
			(x < xOffset + width) &&
			(y >= yOffset) &&
			(y < yOffset + height)) {
			if (!hoveredChildWindows.contains(capturedWindow)) {
				capturedWindow->onMouseHover(x - xOffset, y - yOffset);
				hoveredChildWindows.insert(+capturedWindow);
			} else {
				capturedWindow->onMouseMove(x - xOffset, y - yOffset);
			}
		} else {
			if (hoveredChildWindows.contains(capturedWindow)) {
				capturedWindow->onMouseLeave();
				hoveredChildWindows.remove(capturedWindow);
			}
		}
	} else {
		peff::Map<VirtualWindow *, std::pair<int, int>> childWindows;

		findWindowsAtPos(x, y, childWindows);

		{
			peff::Set<Window *> leftWindows;
			for (auto i : hoveredChildWindows) {
				if (!i->isNative()) {
					if (!childWindows.contains((VirtualWindow *)i)) {
						leftWindows.insert(+i);
					}
				}
			}

			for (auto i : leftWindows) {
				i->onMouseLeave();
				hoveredChildWindows.remove(+i);
			}
		}

		for (auto i = childWindows.begin(); i != childWindows.end(); ++i) {
			if (!hoveredChildWindows.contains(i.key())) {
				i.key()->onMouseHover(x, y);
				hoveredChildWindows.insert(+i.key());
			} else
				i.key()->onMouseMove(i.value().first, i.value().second);
		}
	}
}

CLCWSAL_API void X11Window::onExpose() {
	onDraw();
}

CLCWSAL_API void X11Window::onDraw() {
	int width, height;
	getSize(width, height);

	for (auto i : _childVirtualWindows) {
		int subwindowX, subwindowY;
		int subwindowWidth, subwindowHeight;

		i->getPos(subwindowX, subwindowY);
		i->getSize(subwindowWidth, subwindowHeight);

		i->onDraw();
	}
}

CLCWSAL_API X11Window *X11Window::alloc(
	X11Backend *backend,
	X11WindowHandle nativeHandle) {
	return peff::allocAndConstruct<X11Window>(backend->resourceAllocator.get(), sizeof(std::max_align_t), backend, nativeHandle);
}

CLCWSAL_API void X11Window::findWindowsAtPos(int x, int y, peff::Map<clench::wsal::VirtualWindow *, std::pair<int, int>> &childWindowsOut) {
	for (auto i : _childVirtualWindows) {
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
