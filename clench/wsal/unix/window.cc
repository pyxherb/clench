#include <clench/wsal/window.h>

#include <locale>
#include <map>
#include <stdexcept>

#include "keymap.h"

using namespace clench;
using namespace clench::wsal;

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
			ExposeMask |
			StructureNotifyMask |
			PropertyChangeMask);
}

CLCWSAL_API NativeWindow::~NativeWindow() {
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
			case ButtonPress:
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

				onMouseButtonPress(mappedButton, ev.xbutton.x, ev.xbutton.y);

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
				onMouseLeave(ev.xcrossing.x, ev.xcrossing.y);
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

void NativeWindow::setTitle(std::string_view title) {
	XmbSetWMProperties(
		display,
		nativeHandle,
		title.data(), title.data(),
		nullptr, 0,
		nullptr, nullptr, nullptr);
}
