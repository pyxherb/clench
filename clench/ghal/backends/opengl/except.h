#ifndef _CLENCH_GHAL_BACKENDS_OPENGL_EXCEPT_H_
#define _CLENCH_GHAL_BACKENDS_OPENGL_EXCEPT_H_

#include <clench/ghal/except.h>

namespace clench {
	namespace ghal {
		enum class GLPlatformSpecificExceptionCode : uint32_t {
			IncompatibleWGLDevice = 0,
			ErrorCreatingWGLContext,

			ErrorCreatingEGLSurface,
			ErrorInitializingGLLoader,
		};

		class GLPlatformSpecificException : public PlatformSpecificException {
		public:
			GLPlatformSpecificExceptionCode glPlatformSpecificExceptionCode;

			CLCGHAL_API GLPlatformSpecificException(peff::Alloc *allocator, GLPlatformSpecificExceptionCode glPlatformSpecificExceptionCode);
			CLCGHAL_API virtual ~GLPlatformSpecificException();
		};

		class ErrorCreatingEGLSurfaceException : public GLPlatformSpecificException {
		public:
			CLCGHAL_API ErrorCreatingEGLSurfaceException(peff::Alloc *allocator);
			CLCGHAL_API virtual ~ErrorCreatingEGLSurfaceException();

			CLCGHAL_API const char *what() const override;
			CLCGHAL_API void dealloc() override;

			CLCGHAL_API static ErrorCreatingEGLSurfaceException *alloc(peff::Alloc *allocator);
		};

		class ErrorInitializingGLLoaderException : public GLPlatformSpecificException {
		public:
			CLCGHAL_API ErrorInitializingGLLoaderException(peff::Alloc *allocator);
			CLCGHAL_API virtual ~ErrorInitializingGLLoaderException();

			CLCGHAL_API const char *what() const override;
			CLCGHAL_API void dealloc() override;

			CLCGHAL_API static ErrorInitializingGLLoaderException *alloc(peff::Alloc *allocator);
		};
	}
}

#endif
