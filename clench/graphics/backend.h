#ifndef _CLENCH_GRAPHICS_BACKEND_H_
#define _CLENCH_GRAPHICS_BACKEND_H_

#include <clench/window.h>

#include <deque>

#include "base.h"

namespace clench {
	namespace graphics {
		enum VertexDataType : uint8_t {
			VTXDATA_INT,
			VTXDATA_UINT,
			VTXDATA_SHORT,
			VTXDATA_USHORT,
			VTXDATA_LONG,
			VTXDATA_ULONG,
			VTXDATA_FLOAT,
			VTXDATA_DOUBLE,
			VTXDATA_BOOL
		};

		struct VertexLayout {
			size_t nElements;
			VertexDataType dataType;
			bool normalized;
			ssize_t stride;
			size_t off;

			VertexLayout() = default;
			inline VertexLayout(
				size_t nElements,
				VertexDataType dataType,
				bool normalized,
				ssize_t stride,
				size_t off)
				: nElements(nElements),
				  dataType(dataType),
				  normalized(normalized),
				  stride(stride),
				  off(off) {}
		};

		class GraphicsBackend;
		class GraphicsContext;

		class GraphicsBackend {
		public:
			GraphicsBackend(std::string name);
			virtual ~GraphicsBackend() = default;

			virtual void init() = 0;
			virtual void deinit() = 0;

			virtual GraphicsContext* createGraphicsContext(Window* window) = 0;
		};

		class GraphicsContext {
		public:
			virtual ~GraphicsContext() = default;

			virtual Texture* bakeTexture(RawTexture* rawTexture) = 0;
			virtual Shader* compileShader(RawShader* rawShader) = 0;
			virtual ShaderProgram* linkShader(std::vector<Shader*> shaders) = 0;

			virtual void begin() = 0;
			virtual void end() = 0;
			virtual void present() = 0;

			virtual void onResize(Eigen::Vector2i size) = 0;

			virtual void clear(ColorF color) = 0;

			virtual void setVertexLayout(size_t index, VertexLayout layout) = 0;
			virtual void setVertexLayoutEnabled(size_t index, bool enabled) = 0;

			virtual void setTexture(TextureType type, Texture* texture) = 0;
			virtual void useShaderProgram(ShaderProgram* shaderProgram) = 0;

			virtual void setUniform(size_t index, unsigned int x) = 0;

			virtual void bindVertexBuffer(VertexBuffer* buf) = 0;
			virtual void bindVertexBuffer(IndexedVertexBuffer* buf) = 0;
			virtual void drawIndexedTriangles() = 0;

			virtual VertexBuffer* createVertexBuffer() = 0;
			virtual IndexedVertexBuffer* createIndexedVertexBuffer() = 0;
		};

		void initBackends();
		void deinitBackends();
		GraphicsContext* createGraphicsContext(Window* window, std::deque<std::string> preferredBackends = {});
	}
}

#endif
