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


	std::list<std::string> preferredBackendNames = { "opengl", "d3d11" };
	g_mainGhalDevice = std::unique_ptr<ghal::GHALDevice>(ghal::createGHALDevice(preferredBackendNames));

	if (!g_mainGhalDevice)
		throw std::runtime_error("Error creating main GHAL device");

	g_mainWindow = std::make_unique<MainWindow>();

	g_mainGhalDeviceContext = std::unique_ptr<ghal::GHALDeviceContext>(g_mainGhalDevice->createDeviceContextForWindow(g_mainWindow.get()));

	clench::utils::RcObjectPtr<clench::ghal::VertexShader> vertexShader;
	clench::utils::RcObjectPtr<clench::ghal::FragmentShader> fragmentShader;
	clench::utils::RcObjectPtr<clench::ghal::ShaderProgram> shaderProgram;
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
	clench::ghal::Shader *shaders[] = { vertexShader.get(), fragmentShader.get() };
	shaderProgram = g_mainGhalDevice->linkShaderProgram(shaders, std::size(shaders));

	clench::utils::RcObjectPtr<clench::ghal::VertexArray> vertexArray;
	{
		clench::ghal::VertexArrayElementDesc descs[] = {
			{ clench::ghal::InputVertexShaderSemanticType::Position,
				0,
				0,
				3,
				clench::ghal::VertexDataType::Float,
				sizeof(float) * 4 + sizeof(float) * 3,
				0 },
			{ clench::ghal::InputVertexShaderSemanticType::Color,
				0,
				0,
				4,
				clench::ghal::VertexDataType::Float,
				sizeof(float) * 4 + sizeof(float) * 3,
				sizeof(float) * 3 }
		};

		vertexArray = g_mainGhalDevice->createVertexArray(descs, std::size(descs), vertexShader.get());
	}

	clench::utils::RcObjectPtr<clench::ghal::Buffer> vertexBuffer, indexBuffer;
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

	g_mainWindow->show();

	while (!g_mainWindow->isClosed()) {
		g_mainWindow->pollEvents();

		g_mainGhalDeviceContext->begin();

		g_mainGhalDeviceContext->clearRenderTargetView(nullptr, 0.0f, 0.0f, 0.0f, 1.0f);
		//g_mainGhalDeviceContext->clearDepth(nullptr, 1.0f);
		//g_mainGhalDeviceContext->clearStencil(nullptr, 0);

		g_mainGhalDeviceContext->bindVertexArray(vertexArray.get());
		g_mainGhalDeviceContext->bindVertexBuffer(vertexBuffer.get(), sizeof(float) * 3 + sizeof(float) * 4);
		g_mainGhalDeviceContext->bindIndexBuffer(indexBuffer.get());

		g_mainGhalDeviceContext->setShaderProgram(shaderProgram.get());

		g_mainGhalDeviceContext->drawIndexed(6);

		g_mainGhalDeviceContext->present();

		std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 60));
	}
	indexBuffer.reset();
	vertexBuffer.reset();
	vertexArray.reset();
	shaderProgram.reset();
	fragmentShader.reset();
	vertexShader.reset();

	g_mainGhalDeviceContext.reset();

	g_mainGhalDevice.reset();

	g_mainWindow.reset();

	wsal::deinit();
}
