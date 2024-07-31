#include "window.h"
#include "graphics.h"

using namespace clench;
using namespace clench::engine;

std::unique_ptr<MainWindow> clench::engine::g_mainWindow;

clench::engine::MainWindow::MainWindow()
	: Window(wsi::CREATEWINDOW_RESIZE,
		  "Clench Engine",
		  nullptr,
		  0,
		  0,
		  640,
		  480) {
}

clench::engine::MainWindow::~MainWindow() {
}

void clench::engine::MainWindow::onResize(int width, int height) {
	g_mainRhiDeviceContext->onResize(width, height);
}

void clench::engine::MainWindow::onMove(int x, int y) {
}

bool clench::engine::MainWindow::onClose() {
	return false;
}

void clench::engine::MainWindow::onMouseButtonPress(wsi::MouseButton button, int x, int y) {
}

void clench::engine::MainWindow::onMouseButtonRelease(wsi::MouseButton button, int x, int y) {
}

void clench::engine::MainWindow::onMouseHover(int x, int y) {
}

void clench::engine::MainWindow::onMouseLeave(int x, int y) {
}

void clench::engine::MainWindow::onMouseMove(int x, int y) {
}
