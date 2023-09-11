#ifndef _CLENCH_GRAPHICS_BACKEND_OPENGL_BASE_H_
#define _CLENCH_GRAPHICS_BACKEND_OPENGL_BASE_H_

#include <clench/graphics.h>
#include <glad/glad.h>

#include <stdexcept>

namespace clench {
	namespace graphics {
		class GLBackend : public GraphicsBackend {
		public:
			static void _initGL();
			static void _deinitGL();

			GLBackend();
			virtual ~GLBackend();

			virtual void init() override;
			virtual void deinit() override;

			virtual GraphicsContext* createGraphicsContext(Window* window) override;
		};

		template<typename Base>
		class BasicGLResource : public Base {
		public:
			GLuint handle;

			inline BasicGLResource(GLuint handle) : handle(handle) {}
			virtual inline ~BasicGLResource() {}
		};

		#ifdef _WIN32
		extern HMODULE hOpenGL32Dll;
		#endif
	}
}

#endif
