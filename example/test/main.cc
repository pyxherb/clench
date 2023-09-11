#include <clench/clench.h>

#include <chrono>
#include <clench/utils/stream.hh>
#include <fstream>
#include <thread>

using namespace clench;

int main(int argc, char* argv[]) {
	debug::setupMemoryLeakDetector();
	debug::setupDebugConsole();

	clench::init(0);

	{
		std::unique_ptr<graphics::Window> window(graphics::createWindow(0, L"TH"));
		std::unique_ptr<graphics::GraphicsContext> context(graphics::createGraphicsContext(window.get()));
		window->show();

		std::unique_ptr<graphics::Texture> bakedTexture;
		{
			std::string fileName = "example/test/test.png";
			FILE* fp = fopen(fileName.c_str(), "rb");
			if (!fp) {
				throw std::runtime_error("Error opening file " + fileName);
			}
			std::size_t szBuf;

			char* buf = utils::readAll(fp, szBuf);
			fclose(fp);

			std::unique_ptr<graphics::RawTexture> texture(graphics::decodeImage(buf, szBuf));
			delete[] buf;

			bakedTexture = std::unique_ptr<graphics::Texture>(context->bakeTexture(texture.get()));
		}
		// auto sprite = context->createSprite({ 0.0f, 0.0f, 0.0f }, { 0.75f, 0.75f });
		// sprite->setVisible(true);
		// sprite->setTexture(bakedTexture);

		std::unique_ptr<graphics::FileShader>
			vs = std::make_unique<graphics::FileShader>(graphics::ShaderType::VERTEX, "example/test/sprite.vsh"),
			fs = std::make_unique<graphics::FileShader>(graphics::ShaderType::PIXEL, "example/test/sprite.fsh");

		std::unique_ptr<graphics::ShaderProgram> shader(
			context->linkShader(
				{ std::unique_ptr<graphics::Shader>(context->compileShader(vs.get())).get(),
					std::unique_ptr<graphics::Shader>(context->compileShader(fs.get())).get() }));

		std::unique_ptr<graphics::IndexedVertexBuffer> vertexBuffer(context->createIndexedVertexBuffer());
		vertexBuffer->setVertices(
			{ // Upper right
				0.75f, 0.75f, 0.0f,
				1.0, 0.0,
				1.0, 0.0, 0.0, 1.0,

				// Lower right
				0.75f, 0.0f, 0.0f,
				1.0, 1.0,
				0.0, 1.0, 0.0, 1.0,

				// Lower left
				-0.75f, 0.0f, 0.0f,
				0.0, 1.0,
				0.0, 0.0, 1.0, 1.0,

				// Upper left
				-0.75f, 0.75f, 0.0f,
				0.0, 0.0,
				1.0, 1.0, 1.0, 1.0 },
			graphics::BufferUsage::STATIC_DRAW);
		vertexBuffer->setIndices(
			{ 0, 1, 3,
				1, 2, 3 },
			graphics::BufferUsage::STATIC_DRAW);

		while (!window->isClosed()) {
			auto nextFrameTime = std::chrono::system_clock::now() + std::chrono::milliseconds(1000 / 60);

			context->begin();
			context->clear({ 1.0f, 1.0f, 1.0f, 1.0f });

			// sprite->draw();
			context->setTexture(graphics::TextureType::TEXTURE2D, bakedTexture.get());
			context->useShaderProgram((graphics::ShaderProgram*)shader.get());

			context->bindVertexBuffer(vertexBuffer.get());

			context->setVertexLayout(0, { 3, graphics::VTXDATA_FLOAT, false, 9 * sizeof(float), 0 });
			context->setVertexLayoutEnabled(0, true);

			context->setVertexLayout(1, { 2, graphics::VTXDATA_FLOAT, false, 9 * sizeof(float), sizeof(float) * 3 });
			context->setVertexLayoutEnabled(1, true);

			context->setVertexLayout(2, { 4, graphics::VTXDATA_FLOAT, false, 9 * sizeof(float), sizeof(float) * 5 });
			context->setVertexLayoutEnabled(2, true);

			// context->setUniform(0, 0);
			context->drawIndexedTriangles();

			context->end();
			context->present();
			window->pollEvents();

			while (std::chrono::system_clock::now() < nextFrameTime)
				std::this_thread::yield();
		}

		// sprite.reset();
		vertexBuffer.reset();
		shader.reset();
		bakedTexture.reset();
	}

	clench::deinit();

	return 0;
}
