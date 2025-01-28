#include "backend.h"
#include "keymap.h"

using namespace clench;
using namespace clench::wsal;

X11Backend *wsal::g_x11Backend = nullptr;

CLCWSAL_API bool X11Backend::doInit() {
	if (!builtNativeKeyMap.build(x11KeyMap))
		return false;
	XSetErrorHandler(_x11ErrorHandler);
	return true;
}

CLCWSAL_API bool X11Backend::doDeinit() {
	builtNativeKeyMap.clear();
	return true;
}

CLCWSAL_API X11Backend::X11Backend(
	peff::Alloc *selfAllocator,
	peff::Alloc *resourceAllocator)
	: Backend("x11", selfAllocator, resourceAllocator), operationMutex(this) {
}
CLCWSAL_API X11Backend::~X11Backend() {}

CLCWSAL_API void X11Backend::dealloc() {
	peff::destroyAndRelease<X11Backend>(selfAllocator.get(), this, sizeof(std::max_align_t));
}

CLCWSAL_API int X11Backend::_x11ErrorHandler(Display *display, XErrorEvent *error) {
	switch (error->error_code) {
		case BadAlloc:
			g_x11Backend->lastExceptionPtr = base::wrapIfExceptAllocFailed(base::OutOfMemoryException::alloc());
			break;
		default:
			abort();
	}

	return 0;
}

CLCWSAL_API base::ExceptionPtr X11Backend::createWindow(
	CreateWindowFlags flags,
	Window *parent,
	int x,
	int y,
	int width,
	int height,
	Window *&windowOut) {
	std::lock_guard operationMutexGuard(operationMutex);
	X11WindowHandle nativeHandle;

	Display *display;
	::Window window;

	if (!(nativeHandle.display = (display = XOpenDisplay(nullptr))))
		return base::wrapIfExceptAllocFailed(ErrorOpeningDisplayException::alloc(resourceAllocator.get()));
	peff::ScopeGuard closeDisplayGuard([display]() noexcept {
		XCloseDisplay(display);
	});

	nativeHandle.window = (window = XCreateSimpleWindow(
							   display,
							   parent ? ((X11Window *)parent)->nativeHandle.window : DefaultRootWindow(display),
							   x, y,
							   width,
							   height,
							   0,
							   WhitePixel(display, DefaultScreen(display)),
							   BlackPixel(display, DefaultScreen(display))));
	XFlush(display);
	if (lastExceptionPtr) {
		return base::wrapIfExceptAllocFailed(ErrorCreatingWindowException::alloc(resourceAllocator.get(), lastExceptionPtr.release()));
	}

	peff::ScopeGuard deleteWindowGuard([display, window]() noexcept {
		XEvent ev;
		ev.type = ClientMessage;
		ev.xclient.window = window;
		ev.xclient.message_type = XInternAtom(display, "WM_PROTOCOL", true);
		ev.xclient.format = 32;
		ev.xclient.data.l[0] = XInternAtom(display, "WM_DELETE_WINDOW", false);
		ev.xclient.data.l[1] = CurrentTime;

		XSendEvent(display, window, false, NoEventMask, &ev);

		XFlush(display);
	});

	auto deleteWindowAtom = XInternAtom(display, "WM_DELETE_WINDOW", true);
	XSetWMProtocols(display, window, &deleteWindowAtom, 1);
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

	if (!windowPtr) {
		return base::wrapIfExceptAllocFailed(ErrorCreatingWindowException::alloc(resourceAllocator.get(), base::OutOfMemoryException::alloc()));
	}

	closeDisplayGuard.release();
	deleteWindowGuard.release();

	windowOut = windowPtr.release();
	windowOut->incRef();

	return {};
}

CLCWSAL_API void X11Backend::setMouseCapture(Window *window, Window *childWindow) {
	XGrabPointer(
		((X11Window *)window)->nativeHandle.display,
		((X11Window *)window)->nativeHandle.window,
		true,
		ButtonPressMask | ButtonReleaseMask | PointerMotionMask | FocusChangeMask | EnterWindowMask | LeaveWindowMask,
		GrabModeAsync,
		GrabModeAsync,
		X11_None,
		X11_None,
		CurrentTime);
	((X11Window *)window)->curCapturedWindow = (window == childWindow ? nullptr : childWindow);
}

CLCWSAL_API void X11Backend::releaseMouseCapture(Window *window, Window *childWindow) {
	XUngrabPointer(((X11Window *)window)->nativeHandle.display, CurrentTime);
	((X11Window *)window)->curCapturedWindow = nullptr;
}

CLCWSAL_API X11Backend *X11Backend::alloc(
	peff::Alloc *selfAllocator,
	peff::Alloc *resourceAllocator) {
	return peff::allocAndConstruct<X11Backend>(selfAllocator, sizeof(std::max_align_t), selfAllocator, resourceAllocator);
}
