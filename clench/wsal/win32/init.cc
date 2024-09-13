#include <clench/wsal/init.h>

#define CLENCH_WNDCLASS_NAME "CLENCH"

CLCWSAL_API void clench::wsal::init() {
	WNDCLASS wndClass = {};

	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = NativeWindow::_win32WndProc;
	wndClass.hInstance = GetModuleHandle(NULL);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndClass.lpszClassName = CLENCH_WNDCLASS_NAME;

	if (!RegisterClass(&wndClass))
		throw std::runtime_error("Error registering native window class");
}

CLCWSAL_API void clench::wsal::deinit() {
	UnregisterClass(CLENCH_WNDCLASS_NAME, GetModuleHandle(NULL));
}
