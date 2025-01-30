#ifndef _CLENCH_GHAL_BACKENDS_OPENGL_VERTEX_ARRAY_H_
#define _CLENCH_GHAL_BACKENDS_OPENGL_VERTEX_ARRAY_H_

#include "backend.h"
#include <clench/ghal/vertex_layout.h>

namespace clench {
	namespace ghal {
		class GLVertexLayout : public VertexLayout {
		public:
			GLuint vertexArrayHandle;

			CLENCH_NO_COPY_MOVE_METHODS(GLVertexLayout);

			CLCGHAL_API GLVertexLayout(
				Device *ownerDevice,
				GLuint vertexArrayHandle);
			CLCGHAL_API virtual ~GLVertexLayout();

			CLCGHAL_API virtual void dealloc() override;

			CLCGHAL_API static GLVertexLayout *alloc(
				Device *ownerDevice,
				GLuint vertexArrayHandle);
		};
	}
}

#endif
