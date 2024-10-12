#ifndef _CLENCH_GHAL_BACKENDS_OPENGL_BACKEND_H_
#define _CLENCH_GHAL_BACKENDS_OPENGL_BACKEND_H_

#include <clench/ghal/backend.h>
#if _WIN32
	#include <Windows.h>
#elif __unix__
	#include <EGL/egl.h>
#endif
#include <glad/gl.h>
#include <climits>
#include <map>

namespace clench {
	namespace ghal {
		class GLGHALBackend : public GHALBackend {
		public:
			NO_COPY_MOVE_METHODS(GLGHALBackend);

			CLCGHAL_API GLGHALBackend();
			CLCGHAL_API virtual ~GLGHALBackend();

			CLCGHAL_API virtual GHALDevice *createDevice() override;
		};

#if _WIN32
		CLCGHAL_API extern HMODULE g_hOpenGL32Dll;
#elif __unix__
		CLCGHAL_API extern std::map<EGLDisplay, size_t> g_initializedEglDisplays;
#endif
		CLCGHAL_API extern bool g_glInitialized;

		CLCGHAL_API void *_loadGlProc(const char *name);

		class GLGHALDevice;
	}
}

#endif
