#include "window.h"
#include "backend.h"
#include <clench/utils/logger.h>

using namespace clench;
using namespace clench::wsal;

static wsal::Window *_g_curMouseCapturedWindow = nullptr;
static wsal::Window *_g_curMouseCapturedTopLevelWindow = nullptr;

CLCWSAL_API void clench::wsal::setMouseCapture(Window *window) {
	CLENCH_DEBUG_LOG("WSAL", "Setting mouse capture for window: %p", window);
	if (_g_curMouseCapturedWindow)
		releaseMouseCapture();

	Window *i = window;

	while (i) {
		if (i->backend) {
			_g_curMouseCapturedTopLevelWindow = i;
			_g_curMouseCapturedTopLevelWindow->backend->setMouseCapture(_g_curMouseCapturedTopLevelWindow, _g_curMouseCapturedWindow);
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

	_g_curMouseCapturedTopLevelWindow->backend->releaseMouseCapture(_g_curMouseCapturedTopLevelWindow, _g_curMouseCapturedWindow);
	_g_curMouseCapturedTopLevelWindow = nullptr;
	_g_curMouseCapturedWindow = nullptr;
}

CLCWSAL_API wsal::Window *wsal::getMouseCapture() {
	return _g_curMouseCapturedWindow;
}
