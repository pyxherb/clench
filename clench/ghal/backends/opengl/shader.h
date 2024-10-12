#ifndef _CLENCH_GHAL_BACKENDS_OPENGL_SHADER_H_
#define _CLENCH_GHAL_BACKENDS_OPENGL_SHADER_H_

#include "backend.h"
#include <clench/ghal/shader.h>

namespace clench {
	namespace ghal {
		class GLVertexShader : public VertexShader {
		public:
			GLuint shaderHandle;

			CLCGHAL_API GLVertexShader(
				GHALDevice *ownedDevice,
				GLuint shaderHandle);
			CLCGHAL_API virtual ~GLVertexShader();
		};

		class GLFragmentShader : public FragmentShader {
		public:
			GLuint shaderHandle;

			CLCGHAL_API GLFragmentShader(GHALDevice *ownedDevice, GLuint shaderHandle);
			CLCGHAL_API virtual ~GLFragmentShader();
		};

		class GLGeometryShader : public GeometryShader {
		public:
			GLuint shaderHandle;

			CLCGHAL_API GLGeometryShader(GHALDevice *ownedDevice, GLuint shaderHandle);
			CLCGHAL_API virtual ~GLGeometryShader();
		};

		class GLShaderProgram : public ShaderProgram {
		public:
			GLuint programHandle;
			CLCGHAL_API GLShaderProgram(
				GHALDevice *ownerDevice,
				GLuint programHandle
			);
			CLCGHAL_API virtual ~GLShaderProgram();
		};
	}
}

#endif
