#include "window.h"
#include "graphics.h"
#include <clench/math/mat.h>
#include <thread>
#include <chrono>

using namespace clench;
using namespace clench::engine;

int main(int argc, char** argv) {
	wsi::init();

	rhi::registerBuiltinRHIBackends();

	g_mainRhiDevice = std::unique_ptr<rhi::RHIDevice>(rhi::createRHIDevice());

	if (!g_mainRhiDevice)
		throw std::runtime_error("Error creating main RHI device");

	g_mainWindow = std::make_unique<MainWindow>();

	g_mainRhiDeviceContext = std::unique_ptr<rhi::RHIDeviceContext>(g_mainRhiDevice->createDeviceContextForWindow(g_mainWindow.get()));

	g_mainWindow->show();

	while (!g_mainWindow->isClosed()) {
		g_mainWindow->pollEvents();

		g_mainRhiDeviceContext->clear(1.0f, 0.0f, 1.0f, 1.0f);
		g_mainRhiDeviceContext->clearStencilBuffer(0);
		g_mainRhiDeviceContext->clearDepthBuffer(0.0f);

		g_mainRhiDeviceContext->present();

		std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 60));
	}

	g_mainRhiDeviceContext.reset();

	g_mainRhiDevice.reset();

	g_mainWindow.reset();

	wsi::deinit();
}
