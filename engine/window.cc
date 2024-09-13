#include "window.h"
#include "graphics.h"

using namespace clench;
using namespace clench::engine;

std::unique_ptr<MainWindow> clench::engine::g_mainWindow;

CLCWSAL_API clench::engine::MainWindow::MainWindow()
	: NativeWindow(wsal::CREATEWINDOW_RESIZE,
		  nullptr,
		  0,
		  0,
		  640,
		  480) {
	setTitle("Clench Engine");
}

CLCWSAL_API clench::engine::MainWindow::~MainWindow() {
}

CLCWSAL_API void clench::engine::MainWindow::onResize(int width, int height) {
	g_mainGhalDeviceContext->onResize(width, height);
}

CLCWSAL_API void clench::engine::MainWindow::onMove(int x, int y) {
}

CLCWSAL_API bool clench::engine::MainWindow::onClose() {
	return false;
}

CLCWSAL_API void clench::engine::MainWindow::onMouseButtonPress(wsal::MouseButton button, int x, int y) {
}

CLCWSAL_API void clench::engine::MainWindow::onMouseButtonRelease(wsal::MouseButton button, int x, int y) {
}

CLCWSAL_API void clench::engine::MainWindow::onMouseHover(int x, int y) {
}

CLCWSAL_API void clench::engine::MainWindow::onMouseLeave() {
}

CLCWSAL_API void clench::engine::MainWindow::onMouseMove(int x, int y) {
}

CLCWSAL_API bool clench::engine::MainWindow::isKeyDown(wsal::KeyboardKeyCode keyCode) const {
	return keyboardPressedKeys.count((uint32_t)keyCode);
}
