#include "window.h"
#include "graphics.h"

using namespace clench;
using namespace clench::editor;

peff::RcObjectPtr<wsal::Window> clench::editor::g_mainNativeWindow;
peff::RcObjectPtr<MainWindow> clench::editor::g_mainWindow;

CLCWSAL_API clench::editor::MainWindow::MainWindow(
	peff::Alloc *selfAllocator,
	wsal::Window *parent)
	: VWCRootWindow(
		  selfAllocator,
		  parent,
		  g_mainGhalDevice.get()),
		  keyboardPressedKeys(selfAllocator) {
	setTitle("Clench Engine");
}

CLCWSAL_API clench::editor::MainWindow::~MainWindow() {
}

CLCWSAL_API void clench::editor::MainWindow::onMove(int x, int y) {
}

CLCWSAL_API bool clench::editor::MainWindow::onClose() {
	return false;
}

CLCWSAL_API bool clench::editor::MainWindow::isKeyDown(wsal::KeyboardKeyCode keyCode) const {
	return keyboardPressedKeys.contains((uint32_t)keyCode);
}

CLCWSAL_API MainWindow *MainWindow::alloc(
	peff::Alloc *selfAllocator,
	wsal::Window *parent) {
	return peff::allocAndConstruct<MainWindow>(
		selfAllocator, sizeof(std::max_align_t),
		selfAllocator, parent);
}
