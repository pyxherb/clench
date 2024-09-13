#include "window.h"
#include "graphics.h"
#include <clench/math/mat.h>
#include <clench/utils/stream.h>
#include <thread>
#include <chrono>
#include <fstream>
#include <clench/vwc/button.h>
#include <clench/mod/module.h>

using namespace clench;
using namespace clench::engine;

float vertices[] = {
	0.5f, 0.5f, 0.0f,
	1.0f, 1.0f, 1.0f, 1.0f,

	0.5f, -0.5f, 0.0f,
	1.0f, 1.0f, 0.0f, 1.0f,

	-0.5f, -0.5f, 0.0f,
	1.0f, 0.0f, 1.0f, 1.0f,

	-0.5f, 0.5f, 0.0f,
	0.0f, 1.0f, 1.0f, 1.0f
};

uint32_t indices[] = {
	0, 1, 3,
	1, 2, 3
};

int main(int argc, char **argv) {
	wsal::init();

	ghal::registerBuiltinGHALBackends();

	mod::initBuiltinModules();

	g_mainGhalDevice = std::unique_ptr<ghal::GHALDevice>(ghal::createGHALDevice());

	if (!g_mainGhalDevice)
		throw std::runtime_error("Error creating main GHAL device");

	g_mainWindow = std::make_unique<MainWindow>();

	g_mainGhalDeviceContext = std::unique_ptr<ghal::GHALDeviceContext>(g_mainGhalDevice->createDeviceContextForWindow(g_mainWindow.get()));

	g_mainWindow->show();

	while (!g_mainWindow->isClosed()) {
		g_mainWindow->pollEvents();

		g_mainGhalDeviceContext->begin();

		g_mainGhalDeviceContext->clearRenderTargetView(nullptr, 0.0f, 0.0f, 0.0f, 1.0f);
		g_mainGhalDeviceContext->clearDepth(nullptr, 1.0f);
		g_mainGhalDeviceContext->clearStencil(nullptr, 0);

		g_mainGhalDeviceContext->drawIndexed();

		g_mainGhalDeviceContext->present();

		std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 60));
	}

	mod::deinitModules();

	g_mainGhalDeviceContext.reset();

	g_mainGhalDevice.reset();

	g_mainWindow.reset();

	wsal::deinit();
}
