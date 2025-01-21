#include "backend.h"
#include "keymap.h"

using namespace clench;
using namespace clench::wsal;

CLCWSAL_API bool Win32Backend::doInit() {
	if (!builtNativeKeyMap.build(g_win32KeyMap))
		return false;

	WNDCLASS wndClass = {};

	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = Win32Window::_win32WndProc;
	wndClass.hInstance = GetModuleHandle(NULL);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndClass.lpszClassName = CLENCH_WNDCLASS_NAME;

	if (!RegisterClass(&wndClass))
		return false;

	return true;
}

CLCWSAL_API bool Win32Backend::doDeinit() {
	builtNativeKeyMap.clear();
	UnregisterClass(CLENCH_WNDCLASS_NAME, GetModuleHandle(NULL));
	return true;
}

CLCWSAL_API Win32Backend::Win32Backend(
	peff::Alloc *selfAllocator,
	peff::Alloc *resourceAllocator)
	: Backend("win32", selfAllocator, resourceAllocator) {
}
CLCWSAL_API Win32Backend::~Win32Backend() {}

CLCWSAL_API wsal::Window *Win32Backend::createWindow(
	CreateWindowFlags flags,
	Window *parent,
	int x,
	int y,
	int width,
	int height) {
	Win32WindowHandle nativeHandle;
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
			  parent ? ((Win32Window *)parent)->nativeHandle : NULL,
			  NULL,
			  GetModuleHandle(NULL),
			  0)))
		throw std::runtime_error("Error creating new window");

	std::unique_ptr<Win32Window, peff::RcObjectUniquePtrDeleter> windowPtr(Win32Window::alloc(this, nativeHandle));

	if (!windowPtr) {
		// TODO: Destroy the native window handle;
		return nullptr;
	}

	return windowPtr.release();
}

CLCWSAL_API void Win32Backend::setMouseCapture(Window *window, Window *childWindow) {
	SetCapture(((Win32Window *)window)->nativeHandle);
	((Win32Window *)window)->curCapturedWindow = (window == childWindow ? nullptr : childWindow);
}

CLCWSAL_API void Win32Backend::releaseMouseCapture(Window *window, Window *childWindow) {
	ReleaseCapture();
	((Win32Window *)window)->curCapturedWindow = nullptr;
}

CLCWSAL_API Win32Backend *Win32Backend::alloc(
	peff::Alloc *selfAllocator,
	peff::Alloc *resourceAllocator) {
	return peff::allocAndConstruct<Win32Backend>(selfAllocator, sizeof(std::max_align_t), selfAllocator, resourceAllocator);
}
