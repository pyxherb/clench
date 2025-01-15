#include "window.h"
#include "graphics.h"

using namespace clench;
using namespace clench::engine;

std::unique_ptr<wsal::WindowScope, peff::DeallocableDeleter> clench::engine::g_mainWindowScope;
peff::RcObjectPtr<MainWindow> clench::engine::g_mainWindow;

CLCWSAL_API clench::engine::MainWindow::MainWindow(wsal::WindowScope *windowScope, wsal::NativeWindowHandle nativeWindowHandle)
	: VWCRootWindow(
		  windowScope,
		  nativeWindowHandle,
		  g_mainGhalDevice.get()) {
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

CLCWSAL_API MainWindow *MainWindow::alloc(wsal::WindowScope *windowScope, wsal::NativeWindowHandle nativeWindowHandle) {
	return peff::allocAndConstruct<MainWindow>(
		windowScope->allocator.get(), sizeof(std::max_align_t),
		windowScope, nativeWindowHandle);
}
