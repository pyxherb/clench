#include "window.h"
#include "graphics.h"
#include <clench/math/mat.h>
#include <clench/utils/stream.h>
#include <thread>
#include <chrono>
#include <fstream>
#include <clench/acri/backend.h>
#include <clench/vwc/button.h>

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
	if (!wsal::registerBuiltinWSALBackends(peff::getDefaultAlloc(), peff::getDefaultAlloc()))
		throw std::bad_alloc();
	if (auto result = wsal::scanAndInitRegisteredWSALBackends();
		result.has_value())
		throw std::runtime_error((std::string) "Error initializing WSAL backend " + result->second);

	ghal::registerBuiltinBackends(peff::getDefaultAlloc());
	if (auto result = ghal::scanAndInitRegisteredBackends();
		result.has_value())
		throw std::runtime_error((std::string) "Error initializing GHAL backend " + result->second);

	if(!acri::registerBuiltinBackends(peff::getDefaultAlloc()))
		throw std::bad_alloc();

	peff::List<std::string_view> preferredBackendList;
	if (!preferredBackendList.build({ "opengl" }))
		throw std::bad_alloc();
	{
		ghal::Device *mainGhalDevice;
		if (auto e = ghal::createDevice(mainGhalDevice, preferredBackendList))
			throw std::runtime_error(e->what());
		g_mainGhalDevice = std::unique_ptr<ghal::Device, peff::DeallocableDeleter<ghal::Device>>(mainGhalDevice);
	}

	if (auto e = wsal::createWindow(
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

	{
		acri::Device *mainAcriDevice;
		if(auto e = acri::createDevice(g_mainGhalDevice.get(), peff::getDefaultAlloc(), peff::getDefaultAlloc(), mainAcriDevice))
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

	if (auto result = ghal::deinitInitedRegisteredBackends();
		result.has_value())
		throw std::runtime_error((std::string) "Error deinitializing GHAL backend " + result->second);
	ghal::g_registeredBackends.clear();

	if (auto result = wsal::deinitInitedRegisteredWSALBackends();
		result.has_value())
		throw std::runtime_error((std::string) "Error deinitializing WSAL backend " + result->second);
	wsal::g_registeredWSALBackends.clear();
}
