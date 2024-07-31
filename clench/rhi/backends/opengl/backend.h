#ifndef _CLENCH_RHI_BACKENDS_OPENGL_BACKEND_H_
#define _CLENCH_RHI_BACKENDS_OPENGL_BACKEND_H_

#include <clench/rhi/backend.h>

namespace clench {
	namespace rhi {
		class GLRHIBackend {
		public:
			NO_COPY_MOVE_METHODS(GLRHIBackend);

			CLCRHI_API virtual ~GLRHIBackend();

			virtual const char* getBackendId() = 0;
			virtual RHIBackend* createDevice() = 0;
		};
	}
}

#endif