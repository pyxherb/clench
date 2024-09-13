#ifndef _CLENCH_GHAL_BACKENDS_OPENGL_BACKEND_H_
#define _CLENCH_GHAL_BACKENDS_OPENGL_BACKEND_H_

#include <clench/ghal/backend.h>

namespace clench {
	namespace ghal {
		class GLGHALBackend {
		public:
			NO_COPY_MOVE_METHODS(GLGHALBackend);

			CLCGHAL_API virtual ~GLGHALBackend();

			virtual const char* getBackendId() = 0;
			virtual GHALBackend* createDevice() = 0;
		};
	}
}

#endif
