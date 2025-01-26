#ifndef _CLENCH_GHAL_BACKENDS_OPENGL_BUFFER_H_
#define _CLENCH_GHAL_BACKENDS_OPENGL_BUFFER_H_

#include "backend.h"
#include <clench/ghal/buffer.h>

namespace clench {
	namespace ghal {
		class GLBuffer : public Buffer {
		public:
			GLuint bufferHandle;

			CLENCH_NO_COPY_MOVE_METHODS(GLBuffer);

			CLCGHAL_API GLBuffer(
				GHALDevice *ownerDevice,
				const BufferDesc &bufferDesc,
				GLuint bufferHandle);
			CLCGHAL_API virtual ~GLBuffer();

			CLCGHAL_API virtual void dealloc() override;

			CLCGHAL_API static GLBuffer *alloc(
				GHALDevice *ownerDevice,
				const BufferDesc &bufferDesc,
				GLuint bufferHandle);
		};
	}
}

#endif
