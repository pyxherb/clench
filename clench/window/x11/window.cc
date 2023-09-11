#include "window.h"

#include <clench/debug.h>

#include <locale>
#include <map>
#include <stdexcept>

using namespace clench;
using namespace graphics;

X11WindowBackend x11WindowBackend;

graphics::X11Window::X11Window(CreateWindowFlags flags, std::wstring title) {
	clench::debug::info("Creating a new X11 window...");

	if (!(display = XOpenDisplay(nullptr)))
		throw std::runtime_error("Error opening default display");

	_window = XCreateSimpleWindow(
		display,
		DefaultRootWindow(display),
		0, 0,
		DEFAULT_WINDOW_WIDTH,
		DEFAULT_WINDOW_HEIGHT,
		2,
		WhitePixel(display, DefaultScreen(display)),
		BlackPixel(display, DefaultScreen(display)));

	XSelectInput(
		display, _window,
		ButtonPressMask | ButtonReleaseMask |
			KeyPressMask | KeyReleaseMask |
			StructureNotifyMask |
			VisibilityChangeMask |
			FocusChangeMask |
			EnterWindowMask | LeaveWindowMask |
			PointerMotionMask |
			ResizeRedirectMask |
			ExposureMask |
			ClientMessage);

	auto deleteWindowAtom = XInternAtom(display, "WM_DELETE_WINDOW", true);
	XSetWMProtocols(display, _window, &deleteWindowAtom, 1);
	// XSetErrorHandler(_xErrorHandler);
	// XSetIOErrorHandler(_xIoErrorHandler);
	// XSetIOErrorExitHandler(display, _xIoErrorExitHandler, nullptr);

	clench::debug::info("Created X11 window %#.08x", _window);
}

graphics::X11Window::~X11Window() {
	XDestroyWindow(display, _window);
	XCloseDisplay(display);
}

static std::map<unsigned int, input::KeyboardKeyCode> _keymap = {
	{ XK_space, (input::KeyboardKeyCode)' ' },
	{ XK_exclam, (input::KeyboardKeyCode)'!' },
	{ XK_quotedbl, (input::KeyboardKeyCode)'"' },
	{ XK_numbersign, (input::KeyboardKeyCode)'#' },
	{ XK_dollar, (input::KeyboardKeyCode)'$' },
	{ XK_percent, (input::KeyboardKeyCode)'%' },
	{ XK_ampersand, (input::KeyboardKeyCode)'&' },
	{ XK_quoteright, (input::KeyboardKeyCode)'\'' },
	{ XK_parenleft, (input::KeyboardKeyCode)'(' },
	{ XK_parenright, (input::KeyboardKeyCode)')' },
	{ XK_asterisk, (input::KeyboardKeyCode)'*' },
	{ XK_plus, (input::KeyboardKeyCode)'*' },
	{ XK_comma, (input::KeyboardKeyCode)',' },
	{ XK_minus, (input::KeyboardKeyCode)'-' },
	{ XK_period, (input::KeyboardKeyCode)'.' },
	{ XK_slash, (input::KeyboardKeyCode)'/' },

	{ XK_0, (input::KeyboardKeyCode)'0' },
	{ XK_1, (input::KeyboardKeyCode)'1' },
	{ XK_2, (input::KeyboardKeyCode)'2' },
	{ XK_3, (input::KeyboardKeyCode)'3' },
	{ XK_4, (input::KeyboardKeyCode)'4' },
	{ XK_5, (input::KeyboardKeyCode)'5' },
	{ XK_6, (input::KeyboardKeyCode)'6' },
	{ XK_7, (input::KeyboardKeyCode)'7' },
	{ XK_8, (input::KeyboardKeyCode)'8' },
	{ XK_9, (input::KeyboardKeyCode)'9' },

	{ XK_colon, (input::KeyboardKeyCode)':' },
	{ XK_semicolon, (input::KeyboardKeyCode)';' },
	{ XK_less, (input::KeyboardKeyCode)'<' },
	{ XK_equal, (input::KeyboardKeyCode)'=' },
	{ XK_greater, (input::KeyboardKeyCode)'>' },
	{ XK_question, (input::KeyboardKeyCode)'?' },
	{ XK_at, (input::KeyboardKeyCode)'@' },

	{ XK_A, (input::KeyboardKeyCode)'A' },
	{ XK_B, (input::KeyboardKeyCode)'B' },
	{ XK_C, (input::KeyboardKeyCode)'C' },
	{ XK_D, (input::KeyboardKeyCode)'D' },
	{ XK_E, (input::KeyboardKeyCode)'E' },
	{ XK_F, (input::KeyboardKeyCode)'F' },
	{ XK_G, (input::KeyboardKeyCode)'G' },
	{ XK_H, (input::KeyboardKeyCode)'H' },
	{ XK_I, (input::KeyboardKeyCode)'I' },
	{ XK_J, (input::KeyboardKeyCode)'J' },
	{ XK_K, (input::KeyboardKeyCode)'K' },
	{ XK_L, (input::KeyboardKeyCode)'L' },
	{ XK_M, (input::KeyboardKeyCode)'M' },
	{ XK_N, (input::KeyboardKeyCode)'N' },
	{ XK_O, (input::KeyboardKeyCode)'O' },
	{ XK_P, (input::KeyboardKeyCode)'P' },
	{ XK_Q, (input::KeyboardKeyCode)'Q' },
	{ XK_R, (input::KeyboardKeyCode)'R' },
	{ XK_S, (input::KeyboardKeyCode)'S' },
	{ XK_T, (input::KeyboardKeyCode)'T' },
	{ XK_U, (input::KeyboardKeyCode)'U' },
	{ XK_V, (input::KeyboardKeyCode)'V' },
	{ XK_W, (input::KeyboardKeyCode)'W' },
	{ XK_X, (input::KeyboardKeyCode)'X' },
	{ XK_Y, (input::KeyboardKeyCode)'Y' },
	{ XK_Z, (input::KeyboardKeyCode)'Z' },

	{ XK_bracketleft, (input::KeyboardKeyCode)'[' },
	{ XK_backslash, (input::KeyboardKeyCode)'\\' },
	{ XK_bracketright, (input::KeyboardKeyCode)']' },
	{ XK_asciicircum, (input::KeyboardKeyCode)'^' },
	{ XK_underscore, (input::KeyboardKeyCode)'_' },
	{ XK_quoteleft, (input::KeyboardKeyCode)'`' },

	{ XK_a, (input::KeyboardKeyCode)'a' },
	{ XK_b, (input::KeyboardKeyCode)'b' },
	{ XK_c, (input::KeyboardKeyCode)'c' },
	{ XK_d, (input::KeyboardKeyCode)'d' },
	{ XK_e, (input::KeyboardKeyCode)'e' },
	{ XK_f, (input::KeyboardKeyCode)'f' },
	{ XK_g, (input::KeyboardKeyCode)'g' },
	{ XK_h, (input::KeyboardKeyCode)'h' },
	{ XK_i, (input::KeyboardKeyCode)'i' },
	{ XK_j, (input::KeyboardKeyCode)'j' },
	{ XK_k, (input::KeyboardKeyCode)'k' },
	{ XK_l, (input::KeyboardKeyCode)'l' },
	{ XK_m, (input::KeyboardKeyCode)'m' },
	{ XK_n, (input::KeyboardKeyCode)'n' },
	{ XK_o, (input::KeyboardKeyCode)'o' },
	{ XK_p, (input::KeyboardKeyCode)'p' },
	{ XK_q, (input::KeyboardKeyCode)'q' },
	{ XK_r, (input::KeyboardKeyCode)'r' },
	{ XK_s, (input::KeyboardKeyCode)'s' },
	{ XK_t, (input::KeyboardKeyCode)'t' },
	{ XK_u, (input::KeyboardKeyCode)'u' },
	{ XK_v, (input::KeyboardKeyCode)'v' },
	{ XK_w, (input::KeyboardKeyCode)'w' },
	{ XK_x, (input::KeyboardKeyCode)'x' },
	{ XK_y, (input::KeyboardKeyCode)'y' },
	{ XK_z, (input::KeyboardKeyCode)'z' },

	{ XK_braceleft, (input::KeyboardKeyCode)'{' },
	{ XK_bar, (input::KeyboardKeyCode)'|' },
	{ XK_braceright, (input::KeyboardKeyCode)'}' },
	{ XK_asciitilde, (input::KeyboardKeyCode)'~' },
	{ XK_Delete, input::KBKEY_DEL },

	{ XK_BackSpace, input::KBKEY_BACKSPACE },
	{ XK_Tab, input::KBKEY_TAB },
	{ XK_Clear, input::KBKEY_CLEAR },
	{ XK_Pause, input::KBKEY_PAUSE },
	{ XK_Escape, input::KBKEY_ESC },
	{ XK_Delete, input::KBKEY_DEL },

	{ XK_Up, input::KBKEY_UP },
	{ XK_Down, input::KBKEY_DOWN },
	{ XK_Left, input::KBKEY_LEFT },
	{ XK_Right, input::KBKEY_RIGHT },
	{ XK_Insert, input::KBKEY_INSERT },
	{ XK_Home, input::KBKEY_HOME },
	{ XK_End, input::KBKEY_END },
	{ XK_Page_Up, input::KBKEY_PAGEUP },
	{ XK_Page_Down, input::KBKEY_PAGEDOWN },

	{ XK_KP_0, input::KBKEY_NUMPAD0 },
	{ XK_KP_1, input::KBKEY_NUMPAD1 },
	{ XK_KP_2, input::KBKEY_NUMPAD2 },
	{ XK_KP_3, input::KBKEY_NUMPAD3 },
	{ XK_KP_4, input::KBKEY_NUMPAD4 },
	{ XK_KP_5, input::KBKEY_NUMPAD5 },
	{ XK_KP_6, input::KBKEY_NUMPAD6 },
	{ XK_KP_7, input::KBKEY_NUMPAD7 },
	{ XK_KP_8, input::KBKEY_NUMPAD8 },
	{ XK_KP_9, input::KBKEY_NUMPAD9 },
	{ XK_KP_Add, input::KBKEY_NUMPAD_ADD },
	{ XK_KP_Subtract, input::KBKEY_NUMPAD_SUB },
	{ XK_KP_Multiply, input::KBKEY_NUMPAD_MUL },
	{ XK_KP_Divide, input::KBKEY_NUMPAD_DIV },
	{ XK_KP_Prior, input::KBKEY_NUMPAD_DOT },
	{ XK_KP_Enter, input::KBKEY_NUMPAD_ENTER },
	{ XK_KP_Equal, input::KBKEY_NUMPAD_EQ },

	{ XK_F1, input::KBKEY_F1 },
	{ XK_F2, input::KBKEY_F2 },
	{ XK_F3, input::KBKEY_F3 },
	{ XK_F4, input::KBKEY_F4 },
	{ XK_F5, input::KBKEY_F5 },
	{ XK_F6, input::KBKEY_F6 },
	{ XK_F7, input::KBKEY_F7 },
	{ XK_F8, input::KBKEY_F8 },
	{ XK_F9, input::KBKEY_F9 },
	{ XK_F10, input::KBKEY_F10 },
	{ XK_F11, input::KBKEY_F11 },
	{ XK_F12, input::KBKEY_F12 },

	{ XK_Num_Lock, input::KBKEY_NUM_LOCK },
	{ XK_Caps_Lock, input::KBKEY_CAPS_LOCK },
	{ XK_Scroll_Lock, input::KBKEY_SCROLL_LOCK },
	{ XK_Shift_L, input::KBKEY_LSHIFT },
	{ XK_Shift_R, input::KBKEY_RSHIFT },
	{ XK_Control_L, input::KBKEY_LCTRL },
	{ XK_Control_R, input::KBKEY_RCTRL },
	{ XK_Alt_L, input::KBKEY_LALT },
	{ XK_Alt_R, input::KBKEY_RALT },
	{ XK_Meta_L, input::KBKEY_LMETA },
	{ XK_Meta_R, input::KBKEY_RMETA },
	{ XK_Super_L, input::KBKEY_LSUPER },
	{ XK_Super_R, input::KBKEY_RSUPER },
	{ XK_Mode_switch, input::KBKEY_ALTGR }
};

void graphics::X11Window::pollEvents() {
	XEvent ev;

	while (XPending(display) && (!_isClosed)) {
		XNextEvent(display, &ev);

		switch (ev.type) {
			case KeyPress: {
				auto key = XKeycodeToKeysym(display, ev.xkey.keycode, 0);

				debug::info("Pressed key: 0x%.4x -> 0x%.4x", ev.xkey.keycode, key);

				if (_keymap.count(key)) {
					_keyboardPressedKeys.insert(_keymap.at(key));
				} else
					_keyboardPressedKeys.insert(key);
				break;
			}
			case KeyRelease: {
				auto key = XKeycodeToKeysym(display, ev.xkey.keycode, 0);

				debug::info("Pressed key: 0x%.4x -> 0x%.4x", ev.xkey.keycode, key);

				if (_keymap.count(key)) {
					_keyboardPressedKeys.erase(_keymap.at(key));
				} else
					_keyboardPressedKeys.erase(key);
				break;
			}
			case ButtonPress: {
				break;
			}
			case ButtonRelease: {
				break;
			}
			case ResizeRequest: {
				auto vec = Eigen::Vector2i(ev.xresizerequest.width, ev.xresizerequest.height);
				debug::info("Resizing X11 window %#.08x with size %d * %d", vec.x(), vec.y());
				setSize(vec);
				onResize(vec);
				break;
			}
			case Expose:
				debug::info("X11 window %#.08x exposed", _window);
				XSync(display, false);
				break;
			case ClientMessage: {
				const Atom msg = ev.xclient.data.l[0];

				if (msg == XInternAtom(display, "WM_DELETE_WINDOW", true)) {
					if (this->onClose()) {
						XUnmapWindow(display, _window);
						this->_isClosed = true;
					}
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

void graphics::X11Window::show() {
	XMapWindow(display, _window);
	_isVisible = true;
}

void graphics::X11Window::hide() {
	XUnmapWindow(display, _window);
	_isVisible = false;
}

bool graphics::X11Window::isVisible() const {
	return _isVisible;
}

Eigen::Vector2i graphics::X11Window::getSize() const {
	XWindowAttributes attribs;
	XGetWindowAttributes(display, _window, &attribs);
	return Eigen::Vector2i(attribs.width, attribs.height);
}

void graphics::X11Window::setSize(Eigen::Vector2i size) {
	// XResizeWindow(display, _window, size.x(), size.y());
}

void* graphics::X11Window::getNativeHandle() const {
	return (void*)&_window;
}

WindowBackend& X11Window::getBackend() const {
	return x11WindowBackend;
}

bool X11Window::isKeyDown(input::KeyboardKeyCode keyCode) const {
	return _keyboardPressedKeys.count(keyCode) != 0;
}

unsigned int X11Window::getSystemDpi() const {
	return 96;
}

X11WindowBackend::X11WindowBackend() : WindowBackend("X11") {
}

graphics::Window* X11WindowBackend::createWindow(CreateWindowFlags flags, std::wstring title) {
	return new X11Window(flags, title);
}
