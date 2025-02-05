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
	X11WindowHandle nativeHandle) : NativeWindow(backend), nativeHandle(nativeHandle) {
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
				this->onResize(ev.xconfigure.width, ev.xconfigure.height);

				/*if (ev.xconfigure.width != _width ||
					ev.xconfigure.height != _height)*/

				/*if (ev.xconfigure.x != _x || ev.xconfigure.y != _y)*/
				onMove(ev.xconfigure.x, ev.xconfigure.y);

				[[fallthrough]];
			}
			case Expose:
			case GraphicsExpose:
				this->onExpose();
				XFlush(nativeHandle.display);
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
	return NativeWindow::addChildWindow(window);
}

CLCWSAL_API void X11Window::removeChildWindow(wsal::Window *window) {
	NativeWindow::removeChildWindow(window);

	if (window->isNative()) {
		X11Window *nativeWindow = ((X11Window *)window);

		if (nativeWindow->getParent() != this)
			return;

		::Window rootWindow, parentWindow, *children;
		unsigned int nChildren;

		if (XQueryTree(nativeHandle.display, nativeHandle.window, &rootWindow, &parentWindow, &children, &nChildren))
			return;

		XFree(children);

		XReparentWindow(nativeHandle.display, nativeWindow->nativeHandle.window, rootWindow, 0, 0);
	}
}

CLCWSAL_API bool X11Window::hasChildWindow(wsal::Window *window) const {
	return NativeWindow::hasChildWindow(window);
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

	for (auto i = childVirtualWindows.begin(); i != childVirtualWindows.end(); ++i) {
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

CLCWSAL_API X11Window *X11Window::alloc(
	X11Backend *backend,
	X11WindowHandle nativeHandle) {
	return peff::allocAndConstruct<X11Window>(backend->resourceAllocator.get(), sizeof(std::max_align_t), backend, nativeHandle);
}
