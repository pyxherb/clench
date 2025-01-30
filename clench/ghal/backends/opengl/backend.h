#ifndef _CLENCH_GHAL_BACKENDS_OPENGL_BACKEND_H_
#define _CLENCH_GHAL_BACKENDS_OPENGL_BACKEND_H_

#include <clench/ghal/backend.h>
#include <clench/ghal/device.h>
#if _WIN32
	#include <Windows.h>
#elif __unix__
	#include <EGL/egl.h>
#endif
#include <glad/gl.h>
#include <climits>
#include <map>
#include <thread>

namespace clench {
	namespace ghal {
		class GLBackend : public Backend {
		protected:
			virtual bool doInit() override;
			virtual bool doDeinit() override;

		public:
#ifndef __ANDROID__
			// Explicit reference counter for initialized EGL displays.
			// Android does not require it because Android has its own reference counter,
			// see https://stackoverflow.com/questions/6365366/how-to-properly-initialize-and-terminate-egl-on-android
			CLCGHAL_API peff::Map<EGLDisplay, size_t> initializedEglDisplays;
#endif

			CLENCH_NO_COPY_MOVE_METHODS(GLBackend);

			CLCGHAL_API GLBackend(peff::Alloc *selfAllocator);
			CLCGHAL_API virtual ~GLBackend();

			CLCGHAL_API virtual void dealloc() override;

			CLCGHAL_API virtual base::ExceptionPtr createDevice(Device *&ghalDeviceOut) override;

			CLCGHAL_API static GLBackend *alloc(peff::Alloc *selfAllocator);
		};

#if _WIN32
		CLCGHAL_API extern HMODULE g_hOpenGL32Dll;
#elif __unix__
#endif
		CLCGHAL_API extern bool g_glInitialized;
		CLCGHAL_API extern GLBackend *g_glBackend;

		CLCGHAL_API void *_loadGlProc(const char *name);

		class GLDevice;
	}
}

#endif
