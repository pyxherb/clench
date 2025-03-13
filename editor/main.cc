#include "window.h"
#include "graphics.h"
#include <clench/math/mat.h>
#include <clench/utils/stream.h>
#include <thread>
#include <chrono>
#include <fstream>
#include <clench/acri/backend.h>
#include <clench/vwc/button.h>

#include <clench/ghal/backends/opengl/device.h>

using namespace clench;
using namespace clench::editor;

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
	wsal::WSAL myWsal(peff::getDefaultAlloc());
	if (!myWsal.addBuiltinBackends(peff::getDefaultAlloc())) {
		throw std::bad_alloc();
	}

	wsal::Backend *myWsalBackend = nullptr;
	auto wsalEnumer = [](void *userData, wsal::Backend *backend) -> bool {
		if (!backend->init()) {
			return true;
		}

		*((wsal::Backend **)userData) = backend;
		return false;
	};

	myWsal.enumBackends(&myWsalBackend, wsalEnumer);
	if (!myWsalBackend)
		throw std::runtime_error("No available WSAL backend");

	ghal::GHAL myGhal(peff::getDefaultAlloc());
	if (!myGhal.addBuiltinBackends(peff::getDefaultAlloc())) {
		throw std::bad_alloc();
	}

	ghal::Backend *myGhalBackend = nullptr;
	auto ghalEnumer = [](void *userData, ghal::Backend *backend) -> bool {
		if (!backend->init()) {
			return true;
		}

		*((ghal::Backend **)userData) = backend;
		return false;
	};

	myGhal.enumBackends(&myGhalBackend, ghalEnumer);
	if (!myGhalBackend)
		throw std::runtime_error("No available GHAL backend");

	{
		ghal::Device *mainGhalDevice;
		if (auto e = myGhalBackend->createDevice(mainGhalDevice))
			throw std::runtime_error(e->what());
		g_mainGhalDevice = std::unique_ptr<ghal::Device, peff::DeallocableDeleter<ghal::Device>>(mainGhalDevice);
	}

	if (auto e = myWsalBackend->createWindow(
			wsal::CREATEWINDOW_MIN |
				wsal::CREATEWINDOW_MAX |
				wsal::CREATEWINDOW_RESIZE,
			nullptr,
			0,
			0,
			640,
			480,
			g_mainNativeWindow.getRef());
		e)
		throw std::runtime_error(e->what());
	if (!g_mainNativeWindow)
		throw std::runtime_error("Error creating main native window");
	g_mainNativeWindow->setTitle("Clench Editor");
	g_mainWindow = MainWindow::alloc(peff::getDefaultAlloc(), g_mainNativeWindow.get());
	if (!g_mainWindow)
		throw std::bad_alloc();

	g_mainNativeWindow->show();

	if (!acri::registerBuiltinBackends(peff::getDefaultAlloc())) {
		throw std::runtime_error("Error registering ACRI backends");
	}

	{
		acri::Device *mainAcriDevice;
		if (auto e = acri::createDevice(g_mainGhalDevice.get(), peff::getDefaultAlloc(), peff::getDefaultAlloc(), mainAcriDevice))
			throw std::runtime_error(e->what());
		g_mainAcriDevice = std::unique_ptr<acri::Device, peff::DeallocableDeleter<acri::Device>>(mainAcriDevice);
	}

	peff::RcObjectPtr<acri::DeviceContext> acriDeviceContext;
	acri::createDeviceContext(g_mainWindow->ghalDeviceContext.get(), g_mainAcriDevice.get(), acriDeviceContext.getRef());

	peff::RcObjectPtr<clench::vwc::DefaultButton> button =
		clench::vwc::DefaultButton::alloc(
			peff::getDefaultAlloc(),
			g_mainAcriDevice.get(),
			acriDeviceContext.get(),
			g_mainWindow.get(),
			10,
			10,
			100,
			100);

	button->layoutAttributes = std::make_unique<wsal::LayoutAttributes>();

	button->layoutAttributes->horizontalAlignment = wsal::HorizontalAlignment::Stretch;
	button->layoutAttributes->verticalAlignment = wsal::VerticalAlignment::Stretch;
	button->layoutAttributes->marginBox.left = 100;
	button->layoutAttributes->marginBox.right = 100;
	button->layoutAttributes->marginBox.top = 100;
	button->layoutAttributes->marginBox.bottom = 100;

	while (!g_mainNativeWindow->isClosed()) {
		g_mainNativeWindow->onDraw();
		g_mainNativeWindow->pollEvents();

		// g_mainWindow->onExpose();
		/*
		g_mainGhalDeviceContext->begin();

		g_mainGhalDeviceContext->clearRenderTargetView(nullptr, 0.0f, 0.0f, 0.0f, 1.0f);
		g_mainGhalDeviceContext->clearDepth(nullptr, 1.0f);
		g_mainGhalDeviceContext->clearStencil(nullptr, 0);

		g_mainGhalDeviceContext->bindVertexLayout(vertexArray.get());
		g_mainGhalDeviceContext->bindVertexBuffer(vertexBuffer.get(), sizeof(float) * 3 + sizeof(float) * 4);
		g_mainGhalDeviceContext->bindIndexBuffer(indexBuffer.get());

		g_mainGhalDeviceContext->setVertexShader(vertexShader.get());
		g_mainGhalDeviceContext->setFragmentShader(fragmentShader.get());

		g_mainGhalDeviceContext->drawTriangles();

		g_mainGhalDeviceContext->present();*/

		std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 60));
	}
	button.reset();

	// g_mainGhalDeviceContAext.reset();

	g_mainWindow.reset();
	g_mainNativeWindow.reset();

	acri::g_registeredBackends.clear();

	acriDeviceContext.reset();
	g_mainAcriDevice.reset();
	g_mainGhalDevice.reset();

	if (!myGhalBackend->deinit())
		throw std::runtime_error((std::string) "Error deinitializing the GHAL backend");

	if (!myWsalBackend->deinit())
		throw std::runtime_error((std::string) "Error deinitializing the WSAL backend");
}
