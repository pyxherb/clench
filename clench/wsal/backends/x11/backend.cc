#include "backend.h"
#include "keymap.h"

using namespace clench;
using namespace clench::wsal;

CLCWSAL_API bool X11Backend::doInit() {
	if (!builtNativeKeyMap.build(x11KeyMap))
		return false;
	return true;
}

CLCWSAL_API bool X11Backend::doDeinit() {
	builtNativeKeyMap.clear();
	return true;
}

CLCWSAL_API X11Backend::X11Backend(
	peff::Alloc *selfAllocator,
	peff::Alloc *resourceAllocator)
	: Backend("x11", selfAllocator, resourceAllocator) {
}
CLCWSAL_API X11Backend::~X11Backend() {}

CLCWSAL_API wsal::Window *X11Backend::createWindow(
	CreateWindowFlags flags,
	Window *parent,
	int x,
	int y,
	int width,
	int height) {
	X11WindowHandle nativeHandle;

	Display *display;
	::Window window;

	if (!(nativeHandle.display = (display = XOpenDisplay(nullptr))))
		throw std::runtime_error("Error opening default display");

	nativeHandle.window = (window = XCreateSimpleWindow(
							   display,
							   parent ? ((X11Window*)parent)->nativeHandle.window : DefaultRootWindow(display),
							   x, y,
							   width,
							   height,
							   0,
							   WhitePixel(display, DefaultScreen(display)),
							   BlackPixel(display, DefaultScreen(display))));

	auto deleteWindowAtom = XInternAtom(display, "WM_DELETE_WINDOW", true);
	XSetWMProtocols(display, window, &deleteWindowAtom, 1);
	// XSetErrorHandler(_xErrorHandler);
	// XSetIOErrorHandler(_xIoErrorHandler);
	// XSetIOErrorExitHandler(display, _xIoErrorExitHandler, nullptr);

	XSelectInput(
		display, window,
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

	std::unique_ptr<X11Window, peff::RcObjectUniquePtrDeleter> windowPtr(X11Window::alloc(this, nativeHandle));

	if(!windowPtr) {
		// TODO: Destroy the native window handle;
		return nullptr;
	}

	return windowPtr.release();
}

CLCWSAL_API X11Backend *X11Backend::alloc(
	peff::Alloc *selfAllocator,
	peff::Alloc *resourceAllocator) {
	return peff::allocAndConstruct<X11Backend>(selfAllocator, sizeof(std::max_align_t), selfAllocator, resourceAllocator);
}
