#ifndef _CLENCH_GRAPHICS_VERTEXBUF_H_
#define _CLENCH_GRAPHICS_VERTEXBUF_H_

#include <vector>
#include <cstdint>

namespace clench {
	namespace graphics {
		enum BufferUsage : uint8_t {
			STREAM_DRAW = 0,
			STREAM_READ,
			STREAM_COPY,
			STATIC_DRAW,
			STATIC_READ,
			STATIC_COPY,
			DYNAMIC_DRAW,
			DYNAMIC_READ,
			DYNAMIC_COPY
		};

		class VertexBuffer {
		public:
			virtual ~VertexBuffer() = default;

			virtual void setVertices(std::vector<float> vertices, BufferUsage usage) = 0;
		};

		class IndexedVertexBuffer {
		public:
			virtual ~IndexedVertexBuffer() = default;

			virtual void setVertices(std::vector<float> vertices, BufferUsage usage) = 0;
			virtual void setIndices(std::vector<unsigned int> indices, BufferUsage usage) = 0;
		};
	}
}

#endif
