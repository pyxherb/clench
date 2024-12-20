#ifndef _CLENCH_GHAL_BACKENDS_OPENGL_VERTEX_ARRAY_H_
#define _CLENCH_GHAL_BACKENDS_OPENGL_VERTEX_ARRAY_H_

#include "backend.h"
#include <clench/ghal/vertex_array.h>

namespace clench {
	namespace ghal {
		class GLVertexArray : public VertexArray {
		public:
			GLuint vertexArrayHandle;

			CLENCH_NO_COPY_MOVE_METHODS(GLVertexArray);

			CLCGHAL_API GLVertexArray(
				GHALDevice *ownerDevice,
				GLuint vertexArrayHandle);
			CLCGHAL_API virtual ~GLVertexArray();
		};
	}
}

#endif
