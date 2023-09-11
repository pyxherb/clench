#include "window.h"

#include <comdef.h>

#include <map>
#include <stdexcept>
#include <locale>

using namespace clench;
using namespace clench::graphics;

static WNDCLASSW _wndClass;
constexpr LPCWSTR WNDCLASS_NAME = L"CLENCHMAIN";
Win32Window* _mainWindow = nullptr;

Window* clench::graphics::getMainWindow() {
	return (Window*)_mainWindow;
}

clench::graphics::Win32Window::Win32Window(CreateWindowFlags flags, std::wstring title) {
	clench::debug::info("Creating a new window...");

	if (!(_hWnd = CreateWindowExW(
			  WS_EX_OVERLAPPEDWINDOW,
			  WNDCLASS_NAME,
			  title.c_str(),
			  WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,
			  CW_USEDEFAULT,
			  CW_USEDEFAULT,
			  DEFAULT_WINDOW_WIDTH,
			  DEFAULT_WINDOW_HEIGHT,
			  NULL,
			  NULL,
			  GetModuleHandle(nullptr),
			  NULL))) {
		clench::debug::error("Error creating window with error code %#.08x", GetLastError());
		throw std::runtime_error("Error creating window");
	}

	clench::debug::info("Created window %#.08x", _hWnd);
}

clench::graphics::Win32Window::~Win32Window() {
}

void clench::graphics::Win32Window::pollEvents() {
	MSG msg;
	while (PeekMessage(&msg, _hWnd, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void clench::graphics::Win32Window::show() {
	ShowWindow(_hWnd, SW_SHOW);
}

void clench::graphics::Win32Window::hide() {
	ShowWindow(_hWnd, SW_HIDE);
}

bool clench::graphics::Win32Window::isVisible() {
	return (bool)IsWindowVisible(_hWnd);
}

Eigen::Vector2i clench::graphics::Win32Window::getSize() {
	RECT rect;
	GetClientRect(_hWnd, &rect);
	return Eigen::Vector2i(rect.right - rect.left, rect.bottom - rect.top);
}

void clench::graphics::Win32Window::setSize(Eigen::Vector2i size) {
	RECT rect;
	GetClientRect(_hWnd, &rect);
	rect.right = rect.left + size.x();
	rect.bottom = rect.top + size.y();
	AdjustWindowRect(&rect, 0, false);
}

void* clench::graphics::Win32Window::getNativeHandle() {
	return (void*)&_hWnd;
}

int clench::graphics::getSystemDpi() {
	return GetDpiForSystem();
}

void clench::graphics::initWindow(CreateWindowFlags flags, std::wstring title) {
	clench::debug::info("Initializing window system...");

	if (FAILED(CoInitialize(0))) throw std::runtime_error("Unable to initialize COM");

	_wndClass.lpszClassName = WNDCLASS_NAME;
	_wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	_wndClass.hCursor = LoadIcon((HINSTANCE)GetModuleHandle(NULL), IDC_ARROW);
	_wndClass.hInstance = GetModuleHandle(NULL);
	_wndClass.lpfnWndProc = Win32Window::_wndProc;
	_wndClass.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;

	if (!RegisterClassW(&_wndClass)) {
		clench::debug::info("Error registering window class with error code %#.08x", GetLastError());

		CoUninitialize();
		throw std::runtime_error("Unable to register window class");
	}

	_mainWindow = new Win32Window(flags, title);

	SetProcessDPIAware();  // Disable DPI scaling

	clench::debug::info("Initialized window system");
}

void clench::graphics::deinitWindow() {
	clench::debug::info("Deinitializing window system...");

	delete _mainWindow;

	UnregisterClassW(WNDCLASS_NAME, NULL);
	CoUninitialize();

	clench::debug::info("Deinitialized window system");
}

LRESULT clench::graphics::Win32Window::_wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		case WM_SHOWWINDOW:
			if (wParam)
				clench::debug::info("Showing window %#.08x", hWnd);
			else
				clench::debug::info("Hiding window %#.08x", hWnd);
			break;
		case WM_SIZE:
			clench::debug::info("Resizing window %#.08x with size %hd * %hd", hWnd, LOWORD(lParam), HIWORD(lParam));
			_mainWindow->onResize(clench::Eigen::Vector2i(LOWORD(lParam), HIWORD(lParam)));
			break;
		case WM_MOVE:
			_mainWindow->onMove(clench::Eigen::Vector2i(LOWORD(lParam), HIWORD(lParam)));
			break;
		case WM_CLOSE:
			clench::debug::info("Closing window %#.08x", hWnd);
			if (!(_mainWindow->onClose())) {
				clench::debug::info("Closing of Window %x was cancelled", hWnd);
				break;
			}
		case WM_DESTROY:
			clench::debug::info("Destroying window %#.08x", hWnd);
			_mainWindow->_closed = true;
			break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
