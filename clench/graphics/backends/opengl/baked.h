#ifndef _CLENCH_GRAPHICS_BACKEND_GL_BAKED_H_
#define _CLENCH_GRAPHICS_BACKEND_GL_BAKED_H_

#include "base.h"

namespace clench {
	namespace graphics {
		class GLShader final : public BasicGLResource<Shader> {
		public:
			inline GLShader(GLuint shader) : BasicGLResource(shader) {}
			virtual inline ~GLShader() {
				glDeleteShader(handle);
			}
		};

		class GLShaderProgram final : public BasicGLResource<ShaderProgram> {
		public:
			inline GLShaderProgram(GLuint program) : BasicGLResource(program) {}
			virtual inline ~GLShaderProgram() {
				glDeleteProgram(handle);
			}
		};

		class GLTexture final : public BasicGLResource<Texture> {
		public:
			inline GLTexture(GLuint texture) : BasicGLResource(texture) {}
			virtual inline ~GLTexture() {
				glDeleteTextures(1, &handle);
			}
		};

		inline GLenum toGLBufferUsage(BufferUsage usage) {
			switch (usage) {
				case BufferUsage::STREAM_DRAW:
					return GL_STREAM_DRAW;
				case BufferUsage::STREAM_READ:
					return GL_STREAM_READ;
				case BufferUsage::STREAM_COPY:
					return GL_STREAM_COPY;
				case BufferUsage::STATIC_DRAW:
					return GL_STATIC_DRAW;
				case BufferUsage::STATIC_READ:
					return GL_STATIC_READ;
				case BufferUsage::STATIC_COPY:
					return GL_STATIC_COPY;
				case BufferUsage::DYNAMIC_DRAW:
					return GL_DYNAMIC_DRAW;
				case BufferUsage::DYNAMIC_READ:
					return GL_DYNAMIC_READ;
				case BufferUsage::DYNAMIC_COPY:
					return GL_DYNAMIC_COPY;
				default:
					throw std::runtime_error("Invalid buffer usage");
			}
		}

		class GLVertexBuffer : public VertexBuffer {
		public:
			GLuint vao;
			GLuint vbo;
			size_t szVertices = 0;

			inline GLVertexBuffer() {
				glGenVertexArrays(1, &vao);
				glGenBuffers(1, &vbo);
			}
			virtual inline ~GLVertexBuffer() {
				glDeleteBuffers(1, &vbo);
				glDeleteVertexArrays(1, &vao);
			}

			virtual inline void setVertices(std::vector<float> vertices, BufferUsage usage) override {
				glBindVertexArray(vao);

				glBindBuffer(GL_ARRAY_BUFFER, vbo);
				glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), toGLBufferUsage(usage));

				glBindVertexArray(GL_NONE);

				szVertices = vertices.size();
			}
		};

		class GLIndexedVertexBuffer : public IndexedVertexBuffer {
		public:
			GLuint vao;
			struct {
				GLuint vbo, ebo;
			} buffers;
			size_t szVertices = 0, szIndices = 0;

			inline GLIndexedVertexBuffer() {
				glGenVertexArrays(1, &vao);
				glGenBuffers(sizeof(buffers) / sizeof(GLuint), (GLuint*)&buffers);
			}
			virtual inline ~GLIndexedVertexBuffer() {
				glDeleteBuffers(sizeof(buffers) / sizeof(GLuint), (GLuint*)&buffers);
				glDeleteVertexArrays(1, &vao);
			}

			virtual inline void setVertices(std::vector<float> vertices, BufferUsage usage) override {
				glBindVertexArray(vao);

				glBindBuffer(GL_ARRAY_BUFFER, buffers.vbo);
				glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), toGLBufferUsage(usage));

				glBindVertexArray(GL_NONE);

				szVertices = vertices.size();
			}
			virtual inline void setIndices(std::vector<unsigned int> indices, BufferUsage usage) override {
				glBindVertexArray(vao);

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers.ebo);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), toGLBufferUsage(usage));

				glBindVertexArray(GL_NONE);

				szIndices = indices.size();
			}
		};
	}
}

#endif
