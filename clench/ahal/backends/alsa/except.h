#ifndef _CLENCH_AHAL_BACKENDS_OPENGL_EXCEPT_H_
#define _CLENCH_AHAL_BACKENDS_OPENGL_EXCEPT_H_

#include <clench/ahal/except.h>

namespace clench {
	namespace ahal {
		enum class ALSAPlatformSpecificExceptionCode : uint32_t {
			IncompatibleWALSADevice = 0,
			ErrorCreatingWGLContext,

			ErrorCreatingEGLSurface,
			ErrorInitializingGLLoader,
		};

		class ALSAPlatformSpecificException : public PlatformSpecificException {
		public:
			ALSAPlatformSpecificExceptionCode glPlatformSpecificExceptionCode;

			CLCAHAL_API ALSAPlatformSpecificException(peff::Alloc *allocator, ALSAPlatformSpecificExceptionCode glPlatformSpecificExceptionCode);
			CLCAHAL_API virtual ~ALSAPlatformSpecificException();
		};

		class ErrorCreatingEGLSurfaceException : public ALSAPlatformSpecificException {
		public:
			CLCAHAL_API ErrorCreatingEGLSurfaceException(peff::Alloc *allocator);
			CLCAHAL_API virtual ~ErrorCreatingEGLSurfaceException();

			CLCAHAL_API const char *what() const override;
			CLCAHAL_API void dealloc() override;

			CLCAHAL_API static ErrorCreatingEGLSurfaceException *alloc(peff::Alloc *allocator);
		};

		class ErrorInitializingGLLoaderException : public ALSAPlatformSpecificException {
		public:
			CLCAHAL_API ErrorInitializingGLLoaderException(peff::Alloc *allocator);
			CLCAHAL_API virtual ~ErrorInitializingGLLoaderException();

			CLCAHAL_API const char *what() const override;
			CLCAHAL_API void dealloc() override;

			CLCAHAL_API static ErrorInitializingGLLoaderException *alloc(peff::Alloc *allocator);
		};
	}
}

#endif
