#include <clench/wsi/init.h>

#define CLENCH_WNDCLASS_NAME "CLENCH "

CLCWSI_API void clench::wsi::init() {
	WNDCLASS wndClass = {};

	wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndClass.lpfnWndProc = Window::_win32WndProc;
	wndClass.hInstance = GetModuleHandle(NULL);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndClass.lpszClassName = CLENCH_WNDCLASS_NAME;

	if (!RegisterClass(&wndClass))
		throw std::runtime_error("Error registering native window class");
}

CLCWSI_API void clench::wsi::deinit() {
	UnregisterClass(CLENCH_WNDCLASS_NAME, GetModuleHandle(NULL));
}
