#include "window.h"
#include "backend.h"
#include "keymap.h"
#include <clench/utils/logger.h>

#include <locale>
#include <map>
#include <stdexcept>
#include <thread>

using namespace clench;
using namespace clench::wsal;

CLCWSAL_API LRESULT CALLBACK Win32Window::_win32WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	Win32Window *window = (Win32Window *)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	if (window) {
		Win32Backend *backend = (Win32Backend *)window->backend;

		switch (uMsg) {
			case WM_KEYDOWN: {
				UINT key = wParam;

				if (auto k = backend->builtNativeKeyMap.find(key); k != backend->builtNativeKeyMap.end())
					window->onKeyDown((KeyboardKeyCode)k.value());

				break;
			}
			case WM_KEYUP: {
				UINT key = wParam;

				if (auto k = backend->builtNativeKeyMap.find(key); k != backend->builtNativeKeyMap.end())
					window->onKeyUp((KeyboardKeyCode)k.value());

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

CLCWSAL_API Win32Window::Win32Window(
	Win32Backend *backend,
	Win32WindowHandle nativeHandle) : NativeWindow(backend), nativeHandle(nativeHandle) {
	SetWindowLongPtr(nativeHandle, GWLP_USERDATA, (LONG_PTR)this);
	backend->handleToWindowMap.insert(std::move(nativeHandle), this);
}

CLCWSAL_API Win32Window::~Win32Window() {
	((Win32Backend *)backend)->handleToWindowMap.remove(nativeHandle);

	DestroyWindow(nativeHandle);
}

CLCWSAL_API void Win32Window::dealloc() {
	peff::destroyAndRelease<Win32Window>(backend->resourceAllocator.get(), this, sizeof(std::max_align_t));
}

CLCWSAL_API void Win32Window::show() {
	ShowWindow(nativeHandle, SW_SHOW);
}

CLCWSAL_API void Win32Window::hide() {
	ShowWindow(nativeHandle, SW_HIDE);
}

CLCWSAL_API bool Win32Window::isVisible() const {
	return IsWindowVisible(nativeHandle);
}

CLCWSAL_API bool Win32Window::isClosed() const {
	return _isClosed;
}

CLCWSAL_API void Win32Window::setPos(int x, int y) {
	RECT rect;

	GetClientRect(nativeHandle, &rect);

	MoveWindow(nativeHandle, x, y, rect.right - rect.left, rect.top - rect.bottom, FALSE);
}

CLCWSAL_API void Win32Window::getPos(int &xOut, int &yOut) const {
	RECT rect;
	GetWindowRect(nativeHandle, &rect);

	xOut = rect.left;
	yOut = rect.top;
}

CLCWSAL_API void Win32Window::setSize(int width, int height) {
	RECT rect;

	GetClientRect(nativeHandle, &rect);

	MoveWindow(nativeHandle, rect.left, rect.top, width, height, FALSE);
}

CLCWSAL_API void Win32Window::getSize(int &widthOut, int &heightOut) const {
	RECT rect;

	GetClientRect(nativeHandle, &rect);

	widthOut = rect.right - rect.left;
	heightOut = rect.bottom - rect.top;
}

CLCWSAL_API void Win32Window::setTitle(const char *title) {
	SetWindowTextA(nativeHandle, title);
}

CLCWSAL_API void Win32Window::setParent(Window *window) {
	WindowProperties windowProperties;
	window->getWindowProperties(windowProperties);

	if (!windowProperties.isNative)
		throw std::logic_error("Cannot set parent of a native window to a non-native window");

	SetParent(nativeHandle, ((Win32Window *)window)->nativeHandle);
}

CLCWSAL_API Window *Win32Window::getParent() const {
	Win32Backend *backend = (Win32Backend *)backend;
	HWND parentHandle = GetParent(nativeHandle);

	if (auto it = backend->handleToWindowMap.find(parentHandle);
		it != backend->handleToWindowMap.end())
		return it.value();

	return nullptr;
}

CLCWSAL_API base::ExceptionPtr Win32Window::addChildWindow(Window *window) {
	return NativeWindow::addChildWindow(window);
}

CLCWSAL_API void Win32Window::removeChildWindow(Window *window) {
	if (window->isNative()) {
		WindowProperties windowProperties;
		window->getWindowProperties(windowProperties);

		if (!windowProperties.isNative)
			return;

		HWND hWnd = ((Win32Window *)window)->nativeHandle;

		if (GetParent(hWnd) != nativeHandle)
			return;

		ShowWindow(hWnd, SW_HIDE);

		SetParent(hWnd, NULL);
	} else {
		return NativeWindow::removeChildWindow(window);
	}
}

CLCWSAL_API bool Win32Window::hasChildWindow(Window *window) const {
	if (!window->isNative()) {
		return NativeWindow::hasChildWindow(window);
	}

	return GetParent(((Win32Window *)window)->nativeHandle) == nativeHandle;
}

struct EnumChildWindowsContext {
	Win32Backend *backend;
	ChildWindowEnumer enumer;
};

CLCWSAL_API void Win32Window::enumChildWindows(ChildWindowEnumer &&enumer) {
	Win32Backend *backend = (Win32Backend *)backend;
	EnumChildWindowsContext context;

	context.backend = backend;
	context.enumer = enumer;

	EnumChildWindows(
		nativeHandle,
		[](HWND hWnd, LPARAM lParam) -> BOOL {
			EnumChildWindowsContext *context = ((EnumChildWindowsContext *)lParam);
			if (auto it = context->backend->handleToWindowMap.find(hWnd);
				it != context->backend->handleToWindowMap.end())
				return context->enumer(it.value());
			return true;
		},
		(LPARAM)&context);

	
	for (auto i = childVirtualWindows.begin(); i != childVirtualWindows.end(); ++i) {
		enumer((*i).get());
	}
}

CLCWSAL_API void Win32Window::getWindowProperties(WindowProperties &propertiesOut) const {
	propertiesOut = {};
	propertiesOut.isNative = true;
}

CLCWSAL_API void Win32Window::invalidate() {
	RECT clientRect;
	GetClientRect(nativeHandle, &clientRect);
	InvalidateRect(nativeHandle, &clientRect, false);
}

CLCWSAL_API void Win32Window::pollEvents() {
	MSG msg;

	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

CLCWSAL_API Win32Window *Win32Window::alloc(
	Win32Backend *backend,
	Win32WindowHandle nativeHandle) {
	return peff::allocAndConstruct<Win32Window>(backend->resourceAllocator.get(), sizeof(std::max_align_t), backend, nativeHandle);
}
