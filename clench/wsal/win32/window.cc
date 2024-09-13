#include "window.h"
#include "keymap.h"
#include <clench/utils/logger.h>

#include <locale>
#include <map>
#include <stdexcept>
#include <thread>

using namespace clench;
using namespace clench::wsal;

CLCWSAL_API std::map<HWND, NativeWindow *> clench::wsal::g_win32CreatedWindows;

CLCWSAL_API LRESULT CALLBACK NativeWindow::_win32WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (auto it = g_win32CreatedWindows.find(hWnd); it != g_win32CreatedWindows.end()) {
		auto window = it->second;

		switch (uMsg) {
			case WM_KEYDOWN: {
				UINT key = wParam;

				if (auto k = g_win32KeyMap.find(key); k != g_win32KeyMap.end())
					window->onKeyDown((KeyboardKeyCode)k->second);

				break;
			}
			case WM_KEYUP: {
				UINT key = wParam;

				if (auto k = g_win32KeyMap.find(key); k != g_win32KeyMap.end())
					window->onKeyUp((KeyboardKeyCode)k->second);

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

				if (isRelease)
					window->onMouseButtonRelease(mappedButton, LOWORD(lParam), HIWORD(lParam));
				else
					window->onMouseButtonPress(mappedButton, LOWORD(lParam), HIWORD(lParam));

				break;
			}
			case WM_MOUSEMOVE: {
				int cursorX = GET_X_LPARAM(lParam), cursorY = GET_Y_LPARAM(lParam);

				TRACKMOUSEEVENT tme = {};
				tme.cbSize = sizeof(tme);
				tme.dwFlags = TME_LEAVE;
				tme.hwndTrack = hWnd;
				tme.dwHoverTime = 1;

				TrackMouseEvent(&tme);

				if (!window->isHovered) {
					CLENCH_DEBUG_LOG("WSAL", "Mouse hovered on native window %p with cursor position %d,%d", window, cursorX, cursorY);

					window->onMouseHover(cursorX, cursorY);
					window->isHovered = true;
				} else {
					window->onMouseMove(cursorX, cursorY);
				}
				break;
			}
			case WM_MOUSELEAVE: {
				CLENCH_DEBUG_LOG("WSAL", "Mouse left from native window %p", window);

				window->onMouseLeave();
				window->isHovered = false;

				break;
			}
			case WM_SIZE:
				CLENCH_DEBUG_LOG("WSAL", "Resized native window %p with size %hux%hu", window, LOWORD(lParam), HIWORD(lParam));
				window->onResize(LOWORD(lParam), HIWORD(lParam));
				break;
			case WM_MOVE:
				CLENCH_DEBUG_LOG("WSAL", "Moved native window %p with position %hux%hu", window, LOWORD(lParam), HIWORD(lParam));
				window->onMove(LOWORD(lParam), HIWORD(lParam));
				break;
			case WM_CLOSE:
				if (window->onClose())
					break;
				DestroyWindow(hWnd);
				window->_isClosed = true;
				break;
			case WM_PAINT: {
				window->onExpose();
				break;
			}
		}
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

CLCWSAL_API NativeWindow::NativeWindow(
	CreateWindowFlags flags,
	NativeWindow *parent,
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
			  CLENCH_WNDCLASS_NAME,
			  "",
			  style,
			  x == DEFAULT_WINDOW_POS ? CW_USEDEFAULT : x,
			  y == DEFAULT_WINDOW_POS ? CW_USEDEFAULT : y,
			  width,
			  height,
			  parent ? ((NativeWindow *)parent)->nativeHandle : NULL,
			  NULL,
			  GetModuleHandle(NULL),
			  0)))
		throw std::runtime_error("Error creating new window");

	g_win32CreatedWindows[nativeHandle] = this;
}

CLCWSAL_API NativeWindow::~NativeWindow() {
	g_win32CreatedWindows.erase(nativeHandle);

	DestroyWindow(nativeHandle);
}

CLCWSAL_API void NativeWindow::show() {
	ShowWindow(nativeHandle, SW_SHOW);
}

CLCWSAL_API void NativeWindow::hide() {
	ShowWindow(nativeHandle, SW_HIDE);
}

CLCWSAL_API bool NativeWindow::isVisible() const {
	return IsWindowVisible(nativeHandle);
}

CLCWSAL_API bool NativeWindow::isClosed() const {
	return _isClosed;
}

CLCWSAL_API void NativeWindow::setPos(int x, int y) {
	RECT rect;

	GetClientRect(nativeHandle, &rect);

	MoveWindow(nativeHandle, x, y, rect.right - rect.left, rect.top - rect.bottom, FALSE);
}

CLCWSAL_API void NativeWindow::getPos(int &xOut, int &yOut) const {
	RECT rect;
	GetWindowRect(nativeHandle, &rect);

	xOut = rect.left;
	yOut = rect.top;
}

CLCWSAL_API void NativeWindow::setSize(int width, int height) {
	RECT rect;

	GetClientRect(nativeHandle, &rect);

	MoveWindow(nativeHandle, rect.left, rect.top, width, height, FALSE);
}

CLCWSAL_API void NativeWindow::getSize(int &widthOut, int &heightOut) const {
	RECT rect;

	GetClientRect(nativeHandle, &rect);

	widthOut = rect.right - rect.left;
	heightOut = rect.bottom - rect.top;
}

CLCWSAL_API void NativeWindow::setTitle(const char *title) {
	SetWindowTextA(nativeHandle, title);
}

CLCWSAL_API void NativeWindow::setParent(Window *window) {
	WindowProperties windowProperties;
	window->getWindowProperties(windowProperties);

	if (!windowProperties.isNative)
		throw std::logic_error("Cannot set parent of a native window to a non-native window");

	SetParent(nativeHandle, ((NativeWindow *)window)->nativeHandle);
}

CLCWSAL_API Window *NativeWindow::getParent() const {
	HWND parentHandle = GetParent(nativeHandle);

	if (auto it = g_win32CreatedWindows.find(parentHandle);
		it != g_win32CreatedWindows.end())
		return it->second;

	return nullptr;
}

CLCWSAL_API void NativeWindow::addChildWindow(Window *window) {
	WindowProperties windowProperties;
	window->getWindowProperties(windowProperties);

	if (!windowProperties.isNative)
		throw std::logic_error("Native window accepts native child windows only by default");
}

CLCWSAL_API void NativeWindow::removeChildWindow(Window *window) {
	WindowProperties windowProperties;
	window->getWindowProperties(windowProperties);

	if (!windowProperties.isNative)
		return;

	HWND hWnd = ((NativeWindow *)window)->nativeHandle;

	if (GetParent(hWnd) != nativeHandle)
		return;

	ShowWindow(hWnd, SW_HIDE);

	SetParent(hWnd, NULL);
}

CLCWSAL_API bool NativeWindow::hasChildWindow(Window *window) const {
	WindowProperties windowProperties;
	window->getWindowProperties(windowProperties);

	if (!windowProperties.isNative)
		return false;

	return GetParent(((NativeWindow *)window)->nativeHandle) == nativeHandle;
}

struct EnumChildWindowsContext {
	ChildWindowEnumer enumer;
};

CLCWSAL_API void NativeWindow::enumChildWindows(ChildWindowEnumer &&enumer) {
	EnumChildWindowsContext context;

	context.enumer = enumer;

	EnumChildWindows(
		nativeHandle,
		[](HWND hWnd, LPARAM lParam) -> BOOL {
			if (auto it = g_win32CreatedWindows.find(hWnd);
				it != g_win32CreatedWindows.end())
				return ((EnumChildWindowsContext *)lParam)->enumer(it->second);
			return true;
		},
		(LPARAM)&context);
}

CLCWSAL_API void NativeWindow::getWindowProperties(WindowProperties &propertiesOut) const {
	propertiesOut = {};
	propertiesOut.isNative = true;
}

CLCWSAL_API void NativeWindow::invalidate() {
	RECT clientRect;
	GetClientRect(nativeHandle, &clientRect);
	InvalidateRect(nativeHandle, &clientRect, false);
}

CLCWSAL_API void NativeWindow::pollEvents() {
	MSG msg;

	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
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

static Window *_g_curMouseCapturedWindow = nullptr;
static NativeWindow *_g_curMouseCapturedTopLevelWindow = nullptr;

CLCWSAL_API void clench::wsal::setMouseCapture(Window *window) {
	CLENCH_DEBUG_LOG("WSAL", "Setting mouse capture for window: %p", window);
	if(_g_curMouseCapturedWindow)
		releaseMouseCapture();

	WindowProperties windowProperties;

	window->getWindowProperties(windowProperties);

	Window *i = window->getParent();

	while (i) {
		i->getWindowProperties(windowProperties);

		if (windowProperties.isNative) {
			_g_curMouseCapturedTopLevelWindow = (NativeWindow *)i;
			SetCapture(((NativeWindow *)i)->nativeHandle);
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
	_releaseMouseCapture();

	ReleaseCapture();
}

CLCWSAL_API Window *wsal::getMouseCapture() {
	return _g_curMouseCapturedWindow;
}

CLCWSAL_API void wsal::_releaseMouseCapture() {
	if (_g_curMouseCapturedTopLevelWindow) {
		_g_curMouseCapturedTopLevelWindow->curCapturedWindow = nullptr;
		_g_curMouseCapturedTopLevelWindow = nullptr;
	}
	_g_curMouseCapturedWindow = nullptr;
}
