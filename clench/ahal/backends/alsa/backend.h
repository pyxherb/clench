#ifndef _CLENCH_AHAL_BACKENDS_OPENGL_BACKEND_H_
#define _CLENCH_AHAL_BACKENDS_OPENGL_BACKEND_H_

#include <clench/ahal/backend.h>
#include <clench/ahal/device.h>
#include <climits>
#include <map>
#include <thread>
#include <alsa/asoundlib.h>

namespace clench {
	namespace ahal {
		class ALSABackend : public Backend {
		protected:
			virtual bool doInit() override;
			virtual bool doDeinit() override;

		public:
			CLENCH_NO_COPY_MOVE_METHODS(ALSABackend);

			CLCAHAL_API ALSABackend(peff::Alloc *selfAllocator);
			CLCAHAL_API virtual ~ALSABackend();

			CLCAHAL_API virtual void dealloc() override;

			CLCAHAL_API virtual base::ExceptionPtr createDevice(peff::Alloc *allocator, Device *&ahalDeviceOut) override;

			CLCAHAL_API static ALSABackend *alloc(peff::Alloc *selfAllocator);
		};

#if _WIN32
		CLCAHAL_API extern HMODULE g_hOpenGL32Dll;
#elif __unix__
#endif
		CLCAHAL_API extern ALSABackend *g_alsaBackend;

		CLCAHAL_API void *_loadGlProc(const char *name);

		class ALSADevice;
	}
}

#endif
