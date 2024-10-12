#include <clench/wsal/window.h>
#include <clench/utils/logger.h>
#include <clench/utils/scope_guard.h>

#include <locale>
#include <map>
#include <stdexcept>

#include "keymap.h"

using namespace clench;
using namespace clench::wsal;

static std::map<XID, NativeWindow *> _g_createdWindows;

CLCWSAL_API NativeWindow::NativeWindow(
	CreateWindowFlags flags,
	NativeWindow *parent,
	int x,
	int y,
	int width,
	int height) {
	if (!(display = XOpenDisplay(nullptr)))
		throw std::runtime_error("Error opening default display");

	nativeHandle = XCreateSimpleWindow(
		display,
		parent ? (parent)->nativeHandle : DefaultRootWindow(display),
		x, y,
		width,
		height,
		0,
		WhitePixel(display, DefaultScreen(display)),
		BlackPixel(display, DefaultScreen(display)));

	auto deleteWindowAtom = XInternAtom(display, "WM_DELETE_WINDOW", true);
	XSetWMProtocols(display, nativeHandle, &deleteWindowAtom, 1);
	// XSetErrorHandler(_xErrorHandler);
	// XSetIOErrorHandler(_xIoErrorHandler);
	// XSetIOErrorExitHandler(display, _xIoErrorExitHandler, nullptr);

	XSelectInput(
		display, nativeHandle,
		ButtonPressMask | ButtonReleaseMask | PointerMotionMask | EnterWindowMask | LeaveWindowMask |
			KeyPressMask | KeyReleaseMask |
			StructureNotifyMask |
			VisibilityChangeMask |
			FocusChangeMask |
			EnterWindowMask | LeaveWindowMask |
			PointerMotionMask |
			ResizeRedirectMask |
			ExposureMask |
			StructureNotifyMask |
			PropertyChangeMask);

	_g_createdWindows[nativeHandle] = this;
}

CLCWSAL_API NativeWindow::~NativeWindow() {
	_g_createdWindows.erase(nativeHandle);

	XDestroyWindow(display, nativeHandle);
	XCloseDisplay(display);
}

CLCWSAL_API void NativeWindow::pollEvents() {
	XEvent ev;

	while (XPending(display) && (!_isClosed)) {
		XNextEvent(display, &ev);

		XPointer window;
		XContext context;

		switch (ev.type) {
			case KeyPress: {
				auto key = XLookupKeysym(&ev.xkey, 0);

				if (!_keyPressedTimes.count(key))
					_keyPressedTimes[key] = ev.xkey.time;

				Time timeDiff = ev.xkey.time - _keyPressedTimes[key];

				if ((timeDiff == ev.xkey.time) || ((timeDiff > 0) & (timeDiff < ((Time)0xffffffff)))) {
					_keyPressedTimes[key] = ev.xkey.time;

					if (x11KeyMap.count(key)) {
						onKeyDown((KeyboardKeyCode)x11KeyMap.at(key));
					}
				}
				break;
			}
			case KeyRelease: {
				auto key = XLookupKeysym(&ev.xkey, 0);

				if (XEventsQueued(display, QueuedAfterReading)) {
					XEvent nextEvent;
					XPeekEvent(display, &nextEvent);

					if (nextEvent.type == KeyPress &&
						nextEvent.xkey.window == ev.xkey.window &&
						nextEvent.xkey.keycode == ev.xkey.keycode) {
						if (nextEvent.xkey.time - ev.xkey.time < 20)
							continue;
					}
				}

				if (x11KeyMap.count(key)) {
					onKeyUp((KeyboardKeyCode)x11KeyMap.at(key));
				}
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
				XSync(display, false);
				break;
			case ClientMessage: {
				const Atom msg = ev.xclient.data.l[0];

				if (msg == XInternAtom(display, "WM_DELETE_WINDOW", true)) {
					if (onClose())
						break;

					XUnmapWindow(display, nativeHandle);
					this->_isClosed = true;
					break;
				} else if (msg == XInternAtom(display, "NET_WM_PING", true)) {
					XEvent reply = ev;
					auto root = XRootWindow(display, XDefaultScreen(display));
					reply.xclient.window = root;

					XSendEvent(display, root, False, SubstructureNotifyMask | SubstructureRedirectMask, &reply);
				}
				break;
			}
		}
	}
}

CLCWSAL_API void NativeWindow::show() {
	XMapWindow(display, nativeHandle);
}

CLCWSAL_API void NativeWindow::hide() {
	XUnmapWindow(display, nativeHandle);
}

CLCWSAL_API bool NativeWindow::isVisible() const {
	XWindowAttributes attribs;
	XGetWindowAttributes(display, nativeHandle, &attribs);

	return attribs.map_state == IsViewable;
}

CLCWSAL_API bool NativeWindow::isClosed() const {
	return _isClosed;
}

CLCWSAL_API void NativeWindow::setPos(int x, int y) {
	XMoveWindow(display, nativeHandle, x, y);
}

CLCWSAL_API void NativeWindow::getPos(int &xOut, int &yOut) const {
	XWindowAttributes attribs;

	XGetWindowAttributes(display, nativeHandle, &attribs);

	xOut = attribs.x;
	yOut = attribs.y;
}

CLCWSAL_API void NativeWindow::setSize(int width, int height) {
	XResizeWindow(display, nativeHandle, width, height);
}

CLCWSAL_API void NativeWindow::getSize(int &widthOut, int &heightOut) const {
	XWindowAttributes attribs;

	XGetWindowAttributes(display, nativeHandle, &attribs);

	widthOut = attribs.width;
	heightOut = attribs.height;
}

CLCWSAL_API void NativeWindow::setTitle(const char *title) {
	XmbSetWMProperties(
		display,
		nativeHandle,
		title, title,
		nullptr, 0,
		nullptr, nullptr, nullptr);
}

CLCWSAL_API void NativeWindow::setParent(Window *window) {
	WindowProperties windowProperties;
	window->getWindowProperties(windowProperties);

	if (!windowProperties.isNative)
		throw std::logic_error("Cannot set parent of a native window to a non-native window");

	XReparentWindow(
		((NativeWindow *)window)->display,
		nativeHandle,
		((NativeWindow *)window)->nativeHandle,
		0,
		0);
}

CLCWSAL_API wsal::Window *NativeWindow::getParent() const {
	::Window rootWindow, parentWindow, *children;
	unsigned int nChildren;

	if (XQueryTree(display, nativeHandle, &rootWindow, &parentWindow, &children, &nChildren))
		return nullptr;

	XFree(children);

	if (auto it = _g_createdWindows.find(parentWindow); it != _g_createdWindows.end())
		return it->second;

	return nullptr;
}

CLCWSAL_API void NativeWindow::addChildWindow(wsal::Window *window) {
	WindowProperties windowProperties;
	window->getWindowProperties(windowProperties);

	if (!windowProperties.isNative)
		throw std::logic_error("Native window accepts native child windows only by default");
}

CLCWSAL_API void NativeWindow::removeChildWindow(wsal::Window *window) {
	WindowProperties windowProperties;
	window->getWindowProperties(windowProperties);

	if (!windowProperties.isNative)
		return;

	NativeWindow *nativeWindow = ((NativeWindow *)window);

	if (nativeWindow->getParent() != this)
		return;

	::Window rootWindow, parentWindow, *children;
	unsigned int nChildren;

	if (XQueryTree(display, nativeHandle, &rootWindow, &parentWindow, &children, &nChildren))
		return;

	XFree(children);

	XReparentWindow(display, nativeWindow->nativeHandle, rootWindow, 0, 0);
}

CLCWSAL_API bool NativeWindow::hasChildWindow(wsal::Window *window) const {
	WindowProperties windowProperties;
	window->getWindowProperties(windowProperties);

	if (!windowProperties.isNative)
		return false;

	return ((NativeWindow *)window)->getParent() == this;
}

CLCWSAL_API void NativeWindow::enumChildWindows(ChildWindowEnumer &&enumer) {
	::Window rootWindow, parentWindow, *children;
	unsigned int nChildren;

	if (XQueryTree(display, nativeHandle, &rootWindow, &parentWindow, &children, &nChildren))
		return;

	utils::ScopeGuard freeChildrenGuard([children]() {
		XFree(children);
	});

	for (unsigned int i = 0; i < nChildren; ++i) {
		if (auto it = _g_createdWindows.find(children[i]);
			it != _g_createdWindows.end()) {
			enumer(it->second);
		}
	}
}

CLCWSAL_API void NativeWindow::getWindowProperties(WindowProperties &propertiesOut) const {
	propertiesOut = {};
	propertiesOut.isNative = true;
}

CLCWSAL_API void NativeWindow::invalidate() {
	onExpose();
}

CLCWSAL_API void NativeWindow::onResize(int width, int height) {
}

CLCWSAL_API void NativeWindow::onMove(int x, int y) {
}

CLCWSAL_API bool NativeWindow::onClose() {
	return false;
}

CLCWSAL_API void NativeWindow::onKeyDown(KeyboardKeyCode keyCode) {
}

CLCWSAL_API void NativeWindow::onKeyUp(KeyboardKeyCode keyCode) {
}

CLCWSAL_API void NativeWindow::onMouseButtonPress(MouseButton button, int x, int y) {
}

CLCWSAL_API void NativeWindow::onMouseButtonRelease(MouseButton button, int x, int y) {
}

CLCWSAL_API void NativeWindow::onMouseHover(int x, int y) {
}

CLCWSAL_API void NativeWindow::onMouseLeave() {
}

CLCWSAL_API void NativeWindow::onMouseMove(int x, int y) {
}

CLCWSAL_API void NativeWindow::onExpose() {
	onDraw();
}

CLCWSAL_API void NativeWindow::onDraw() {
}

static wsal::Window *_g_curMouseCapturedWindow = nullptr;
static NativeWindow *_g_curMouseCapturedTopLevelWindow = nullptr;

CLCWSAL_API void clench::wsal::setMouseCapture(Window *window) {
	CLENCH_DEBUG_LOG("WSAL", "Setting mouse capture for window: %p", window);
	if (_g_curMouseCapturedWindow)
		releaseMouseCapture();

	WindowProperties windowProperties;

	window->getWindowProperties(windowProperties);

	Window *i = window->getParent();

	while (i) {
		i->getWindowProperties(windowProperties);

		if (windowProperties.isNative) {
			_g_curMouseCapturedTopLevelWindow = (NativeWindow *)i;
			XGrabPointer(
				((NativeWindow *)i)->display,
				((NativeWindow *)i)->nativeHandle,
				true,
				ButtonPressMask | ButtonReleaseMask | PointerMotionMask | FocusChangeMask | EnterWindowMask | LeaveWindowMask,
				GrabModeAsync,
				GrabModeAsync,
				X11_None,
				X11_None,
				CurrentTime);
			((NativeWindow *)i)->curCapturedWindow = i == window ? nullptr : window;
			goto foundTopLevelWindow;
		}

		i = i->getParent();
	}

	throw std::logic_error("Window to be captured must have a native parent window on the window chain");

foundTopLevelWindow:

	_g_curMouseCapturedWindow = window;
}

CLCWSAL_API void clench::wsal::releaseMouseCapture() {
	CLENCH_DEBUG_LOG("WSAL", "Releasing mouse capture");

	XUngrabPointer(_g_curMouseCapturedTopLevelWindow->display, CurrentTime);
	_releaseMouseCapture();
}

CLCWSAL_API wsal::Window *wsal::getMouseCapture() {
	return _g_curMouseCapturedWindow;
}

CLCWSAL_API void wsal::_releaseMouseCapture() {
	if (_g_curMouseCapturedTopLevelWindow) {
		_g_curMouseCapturedTopLevelWindow->curCapturedWindow = nullptr;
		_g_curMouseCapturedTopLevelWindow = nullptr;
	}
	_g_curMouseCapturedWindow = nullptr;
}
