#include "window.h"
#include "graphics.h"

using namespace clench;
using namespace clench::engine;

peff::RcObjectPtr<wsal::Window> clench::engine::g_mainNativeWindow;
peff::RcObjectPtr<MainWindow> clench::engine::g_mainWindow;

CLCWSAL_API clench::engine::MainWindow::MainWindow(
	peff::Alloc *selfAllocator,
	wsal::Window *parent)
	: VWCRootWindow(
		  selfAllocator,
		  parent,
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

CLCWSAL_API MainWindow *MainWindow::alloc(
	peff::Alloc *selfAllocator,
	wsal::Window *parent) {
	return peff::allocAndConstruct<MainWindow>(
		selfAllocator, sizeof(std::max_align_t),
		selfAllocator, parent);
}
