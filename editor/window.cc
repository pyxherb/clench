#include "window.h"
#include "graphics.h"

using namespace clench;
using namespace clench::engine;

std::unique_ptr<wsal::WindowScope, peff::DeallocableDeleter> clench::engine::g_mainWindowScope;
peff::RcObjectPtr<MainWindow> clench::engine::g_mainWindow;

CLCWSAL_API clench::engine::MainWindow::MainWindow(wsal::WindowScope *windowScope)
	: VWCRootWindow(
		  windowScope,
		  wsal::CREATEWINDOW_MIN |
			  wsal::CREATEWINDOW_MAX |
			  wsal::CREATEWINDOW_RESIZE,
		  g_mainGhalDevice.get(),
		  nullptr,
		  0,
		  0,
		  640,
		  480) {
	setTitle("Clench Engine");
}

CLCWSAL_API clench::engine::MainWindow::~MainWindow() {
}

CLCWSAL_API void clench::engine::MainWindow::onMove(int x, int y) {
}

CLCWSAL_API bool clench::engine::MainWindow::onClose() {
	return false;
}

CLCWSAL_API bool clench::engine::MainWindow::isKeyDown(wsal::KeyboardKeyCode keyCode) const {
	return keyboardPressedKeys.count((uint32_t)keyCode);
}
