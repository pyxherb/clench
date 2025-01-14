#include "window.h"
#include "graphics.h"
#include <clench/math/mat.h>
#include <clench/utils/stream.h>
#include <thread>
#include <chrono>
#include <fstream>
#include <clench/vwc/button.h>

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

	g_mainGhalDevice = std::unique_ptr<ghal::GHALDevice, peff::DeallocableDeleter>(ghal::createGHALDevice({ "opengl" }));

	if (!g_mainGhalDevice)
		throw std::runtime_error("Error creating main GHAL device");

	g_mainWindowScope = std::unique_ptr<wsal::WindowScope, peff::DeallocableDeleter>(
		wsal::WindowScope::alloc(peff::getDefaultAlloc(), peff::getDefaultAlloc()));
	if (!g_mainWindowScope)
		throw std::bad_alloc();

	g_mainWindow = MainWindow::alloc(g_mainWindowScope.get(),
		wsal::createNativeWindow(
			wsal::CREATEWINDOW_MIN |
				wsal::CREATEWINDOW_MAX |
				wsal::CREATEWINDOW_RESIZE,
			nullptr,
			0,
			0,
			640,
			480));
	if (!g_mainWindow)
		throw std::bad_alloc();

	g_mainWindow->show();

	peff::RcObjectPtr<clench::ghal::VertexShader> vertexShader;
	peff::RcObjectPtr<clench::ghal::FragmentShader> fragmentShader;
	{
		std::ifstream is("test_vertex.glsl");

		is.seekg(0, std::ios::end);
		size_t size = is.tellg();
		is.seekg(0, std::ios::beg);

		std::unique_ptr<char[]> vsSrc(std::make_unique<char[]>(size));
		is.read(vsSrc.get(), size);

		vertexShader = g_mainGhalDevice->createVertexShader(vsSrc.get(), size, nullptr);
	}
	{
		std::ifstream is("test_frag.glsl");

		is.seekg(0, std::ios::end);
		size_t size = is.tellg();
		is.seekg(0, std::ios::beg);

		std::unique_ptr<char[]> fsSrc(std::make_unique<char[]>(size));
		is.read(fsSrc.get(), size);

		fragmentShader = g_mainGhalDevice->createFragmentShader(fsSrc.get(), size, nullptr);
	}

	peff::RcObjectPtr<clench::ghal::VertexArray> vertexArray;
	{
		clench::ghal::VertexArrayElementDesc descs[] = {
			{ clench::ghal::InputVertexShaderSemanticType::Position,
				0,
				3,
				clench::ghal::VertexDataType::Float,
				sizeof(float) * 4 + sizeof(float) * 3,
				0 },
			{ clench::ghal::InputVertexShaderSemanticType::Color,
				0,
				4,
				clench::ghal::VertexDataType::Float,
				sizeof(float) * 4 + sizeof(float) * 3,
				sizeof(float) * 3 }
		};

		vertexArray = g_mainGhalDevice->createVertexArray(descs, std::size(descs), vertexShader.get());
	}

	peff::RcObjectPtr<clench::ghal::Buffer> vertexBuffer, indexBuffer;
	{
		clench::ghal::BufferDesc vertexBufferDesc, indexBufferDesc;

		vertexBufferDesc.size = sizeof(vertices);
		vertexBufferDesc.usage = clench::ghal::BufferUsage::Static;
		vertexBufferDesc.proposedTarget = clench::ghal::BufferTarget::Vertex;
		vertexBufferDesc.cpuReadable = false;
		vertexBufferDesc.cpuWritable = true;
		vertexBuffer = g_mainGhalDevice->createBuffer(vertexBufferDesc, vertices);

		indexBufferDesc.size = sizeof(indices);
		indexBufferDesc.usage = clench::ghal::BufferUsage::Static;
		indexBufferDesc.proposedTarget = clench::ghal::BufferTarget::Index;
		indexBufferDesc.cpuReadable = false;
		indexBufferDesc.cpuWritable = true;
		indexBuffer = g_mainGhalDevice->createBuffer(indexBufferDesc, indices);
	}

	peff::RcObjectPtr<clench::vwc::DefaultButton> button =
		new clench::vwc::DefaultButton(
			g_mainWindowScope.get(),
			g_mainGhalDevice.get(),
			g_mainWindow->ghalDeviceContext.get(),
			clench::ghal::TextureFormat::RGBA8,
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

	while (!g_mainWindow->isClosed()) {
		g_mainWindow->onDraw();
		g_mainWindow->pollEvents();

		// g_mainWindow->onExpose();
		/*
		g_mainGhalDeviceContext->begin();

		g_mainGhalDeviceContext->clearRenderTargetView(nullptr, 0.0f, 0.0f, 0.0f, 1.0f);
		g_mainGhalDeviceContext->clearDepth(nullptr, 1.0f);
		g_mainGhalDeviceContext->clearStencil(nullptr, 0);

		g_mainGhalDeviceContext->bindVertexArray(vertexArray.get());
		g_mainGhalDeviceContext->bindVertexBuffer(vertexBuffer.get(), sizeof(float) * 3 + sizeof(float) * 4);
		g_mainGhalDeviceContext->bindIndexBuffer(indexBuffer.get());

		g_mainGhalDeviceContext->setVertexShader(vertexShader.get());
		g_mainGhalDeviceContext->setFragmentShader(fragmentShader.get());

		g_mainGhalDeviceContext->drawTriangles();

		g_mainGhalDeviceContext->present();*/

		std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 60));
	}
	button.reset();
	indexBuffer.reset();
	vertexBuffer.reset();
	vertexArray.reset();
	fragmentShader.reset();
	vertexShader.reset();

	// g_mainGhalDeviceContext.reset();

	g_mainWindow.reset();
	g_mainWindowScope.reset();

	g_mainGhalDevice.reset();

	wsal::deinit();
}
