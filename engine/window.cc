#include "window.h"
#include "graphics.h"

using namespace clench;
using namespace clench::engine;

std::unique_ptr<MainWindow> clench::engine::g_mainWindow;

clench::engine::MainWindow::MainWindow()
	: NativeWindow(wsal::CREATEWINDOW_RESIZE,
		  nullptr,
		  0,
		  0,
		  640,
		  480) {
	setTitle("Clench Engine");
}

clench::engine::MainWindow::~MainWindow() {
}

void clench::engine::MainWindow::onResize(int width, int height) {
	g_mainGhalDeviceContext->onResize(width, height);
}

void clench::engine::MainWindow::onMove(int x, int y) {
}

bool clench::engine::MainWindow::onClose() {
	return false;
}

void clench::engine::MainWindow::onMouseButtonPress(wsal::MouseButton button, int x, int y) {
}

void clench::engine::MainWindow::onMouseButtonRelease(wsal::MouseButton button, int x, int y) {
}

void clench::engine::MainWindow::onMouseHover(int x, int y) {
}

void clench::engine::MainWindow::onMouseLeave() {
}

void clench::engine::MainWindow::onMouseMove(int x, int y) {
}

bool clench::engine::MainWindow::isKeyDown(wsal::KeyboardKeyCode keyCode) const {
	return keyboardPressedKeys.count((uint32_t)keyCode);
}
