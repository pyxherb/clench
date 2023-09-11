#ifndef _CLENCH_GRAPHICS_BACKEND_OPENGL_BACKEND_H_
#define _CLENCH_GRAPHICS_BACKEND_OPENGL_BACKEND_H_

#include <clench/debug.h>

#include <list>

#include "baked.h"

#if _WIN32
#else
	#include <EGL/egl.h>
#endif

namespace clench {
	namespace graphics {
		class GLSprite;

		class GLContext final : public GraphicsContext {
		protected:
		public:
			Window* _window;
#ifdef _WIN32
			HDC _hdc;
			HGLRC _wglContext;
#else
			EGLDisplay _eglDisplay;
			EGLint _eglMinor, _eglMajor;
			EGLConfig _eglConfig;
			EGLSurface _eglSurface;
			EGLContext _eglContext;
			EGLNativeWindowType _eglWindow;
#endif

			size_t _szIndices = 0;

			GLContext(Window* window);
			virtual ~GLContext();

			virtual Texture* bakeTexture(RawTexture* rawTexture) override;
			virtual Shader* compileShader(RawShader* rawShader) override;
			virtual ShaderProgram* linkShader(std::vector<Shader*> rawShader) override;

			virtual void begin() override;
			virtual void end() override;
			virtual void present() override;

			virtual void onResize(Eigen::Vector2i size) override;

			virtual void clear(ColorF color) override;

			virtual void setTexture(TextureType type, Texture* texture) override;
			virtual void useShaderProgram(ShaderProgram* shaderProgram) override;

			virtual void setVertexLayout(size_t index, VertexLayout layout) override;
			virtual void setVertexLayoutEnabled(size_t index, bool enabled) override;

			virtual void setUniform(size_t index, unsigned int x) override;

			virtual void bindVertexBuffer(VertexBuffer* buf) override;
			virtual void bindVertexBuffer(IndexedVertexBuffer* buf) override;

			virtual VertexBuffer* createVertexBuffer() override;
			virtual IndexedVertexBuffer* createIndexedVertexBuffer() override;

			virtual void drawIndexedTriangles() override;
		};
	}
}

#endif
