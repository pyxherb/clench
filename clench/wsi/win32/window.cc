#include "window.h"
#include "keymap.h"

#include <locale>
#include <map>
#include <stdexcept>

using namespace clench;
using namespace clench::wsi;

CLCWSI_API std::map<HWND, Window *> clench::wsi::g_win32CreatedWindows;

CLCWSI_API LRESULT CALLBACK Window::_win32WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (auto it = g_win32CreatedWindows.find(hWnd); it != g_win32CreatedWindows.end()) {
		auto window = it->second;

		switch (uMsg) {
			case WM_KEYDOWN: {
				UINT key = wParam;

				if (auto k = g_win32KeyMap.find(key); k != g_win32KeyMap.end())
					window->keyboardPressedKeys.insert((uint32_t)k->second);

				break;
			}
			case WM_KEYUP: {
				UINT key = wParam;

				if (auto k = g_win32KeyMap.find(key); k != g_win32KeyMap.end())
					window->keyboardPressedKeys.erase((uint32_t)k->second);

				break;
			}
			case WM_LBUTTONUP:
			case WM_LBUTTONDOWN:
			case WM_RBUTTONUP:
			case WM_RBUTTONDOWN:
			case WM_MBUTTONUP:
			case WM_MBUTTONDOWN: {
				MouseButton mappedButton;
				bool isRelease = false;

				switch (uMsg) {
					case WM_LBUTTONDOWN:
					case WM_LBUTTONUP:
						mappedButton = MouseButton::Left;
						isRelease = (uMsg == WM_LBUTTONUP);
						break;
					case WM_MBUTTONDOWN:
					case WM_MBUTTONUP:
						mappedButton = MouseButton::Middle;
						isRelease = (uMsg == WM_MBUTTONUP);
						break;
					case WM_RBUTTONDOWN:
					case WM_RBUTTONUP:
						mappedButton = MouseButton::Right;
						isRelease = (uMsg == WM_RBUTTONUP);
						break;
					default:
						assert(false);
				}

				break;
			}
			case WM_MOUSEMOVE: {
				window->onMouseMove(LOWORD(lParam), HIWORD(lParam));
				break;
			}
			case WM_MOUSEHOVER: {
				window->onMouseHover(LOWORD(lParam), HIWORD(lParam));

				// TODO: Pass the message to child windows.

				TRACKMOUSEEVENT tme = {};
				tme.cbSize = sizeof(tme);
				tme.dwFlags = TME_LEAVE;
				tme.hwndTrack = hWnd;
				tme.dwHoverTime = 1;

				TrackMouseEvent(&tme);

				break;
			}
			case WM_MOUSELEAVE: {
				window->onMouseLeave(LOWORD(lParam), HIWORD(lParam));

				// TODO: Pass the message to child windows.

				TRACKMOUSEEVENT tme = {};
				tme.cbSize = sizeof(tme);
				tme.dwFlags = TME_HOVER;
				tme.hwndTrack = hWnd;
				tme.dwHoverTime = 1;

				TrackMouseEvent(&tme);

				break;
			}
			case WM_SIZE:
				window->onResize(LOWORD(lParam), HIWORD(lParam));
				break;
			case WM_MOVE:
				window->onResize(LOWORD(lParam), HIWORD(lParam));
				break;
			case WM_CLOSE:
				if (window->onClose())
					break;
				DestroyWindow(hWnd);
				window->_isClosed = true;
				break;
		}
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

CLCWSI_API Window::Window(
	CreateWindowFlags flags,
	const char *title,
	Window *parent,
	int x,
	int y,
	int width,
	int height) {
	DWORD style = parent ? WS_CHILDWINDOW : WS_OVERLAPPEDWINDOW;

	if (!(flags & CREATEWINDOW_MIN))
		style &= ~WS_MINIMIZEBOX;

	if (!(flags & CREATEWINDOW_MAX))
		style &= ~WS_MAXIMIZEBOX;

	if (!(flags & CREATEWINDOW_RESIZE))
		style &= ~WS_SIZEBOX;

	if (flags & CREATEWINDOW_NOFRAME)
		style &= ~WS_THICKFRAME;

	if (!(nativeHandle = CreateWindow(
			  CLCWSI_WNDCLASS_NAME,
			  title,
			  style,
			  x == DEFAULT_WINDOW_POS ? CW_USEDEFAULT : x,
			  y == DEFAULT_WINDOW_POS ? CW_USEDEFAULT : y,
			  width,
			  height,
			  parent ? ((Window *)parent)->nativeHandle : NULL,
			  NULL,
			  GetModuleHandle(NULL),
			  0)))
		throw std::runtime_error("Error creating new window");

	setTitle(title);

	g_win32CreatedWindows[nativeHandle] = this;
}

CLCWSI_API Window::~Window() {
	g_win32CreatedWindows.erase(nativeHandle);

	DestroyWindow(nativeHandle);
}

CLCWSI_API void Window::show() {
	ShowWindow(nativeHandle, SW_SHOW);
}

CLCWSI_API void Window::hide() {
	ShowWindow(nativeHandle, SW_HIDE);
}

CLCWSI_API bool Window::isVisible() const {
	return IsWindowVisible(nativeHandle);
}

CLCWSI_API bool Window::isClosed() const {
	return _isClosed;
}

CLCWSI_API void Window::setPos(int x, int y) {
	RECT rect;

	GetClientRect(nativeHandle, &rect);

	MoveWindow(nativeHandle, x, y, rect.right - rect.left, rect.top - rect.bottom, FALSE);
}

CLCWSI_API void Window::getPos(int &xOut, int &yOut) const {
	RECT rect;
	GetWindowRect(nativeHandle, &rect);

	xOut = rect.left;
	yOut = rect.top;
}

CLCWSI_API void Window::setSize(int width, int height) {
	RECT rect;

	GetClientRect(nativeHandle, &rect);

	MoveWindow(nativeHandle, rect.left, rect.top, width, height, FALSE);
}

CLCWSI_API void Window::getSize(int &widthOut, int &heightOut) const {
	RECT rect;

	GetClientRect(nativeHandle, &rect);

	widthOut = rect.right - rect.left;
	heightOut = rect.bottom - rect.top;
}

CLCWSI_API bool Window::isKeyDown(KeyboardKeyCode keyCode) const {
	return keyboardPressedKeys.count((uint32_t)keyCode);
}

void Window::setTitle(const char *title) {
	SetWindowTextA(nativeHandle, title);
}

CLCWSI_API void Window::pollEvents() {
	MSG msg;

	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}
