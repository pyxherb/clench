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
				Device *ownedDevice,
				GLuint shaderHandle);
			CLCGHAL_API virtual ~GLVertexShader();

			CLCGHAL_API virtual void dealloc() override;

			CLCGHAL_API static GLVertexShader *alloc(
				Device *ownedDevice,
				GLuint shaderHandle
			);
		};

		class GLFragmentShader : public FragmentShader {
		public:
			GLuint shaderHandle;

			CLCGHAL_API GLFragmentShader(Device *ownedDevice, GLuint shaderHandle);
			CLCGHAL_API virtual ~GLFragmentShader();

			CLCGHAL_API virtual void dealloc() override;

			CLCGHAL_API static GLFragmentShader *alloc(
				Device *ownedDevice,
				GLuint shaderHandle
			);
		};

		class GLGeometryShader : public GeometryShader {
		public:
			GLuint shaderHandle;

			CLCGHAL_API GLGeometryShader(Device *ownedDevice, GLuint shaderHandle);
			CLCGHAL_API virtual ~GLGeometryShader();

			CLCGHAL_API virtual void dealloc() override;

			CLCGHAL_API static GLGeometryShader *alloc(
				Device *ownedDevice,
				GLuint shaderHandle
			);
		};

		class GLShaderProgram : public ShaderProgram {
		public:
			GLuint programHandle;

			CLCGHAL_API GLShaderProgram(
				Device *ownerDevice,
				GLuint programHandle
			);
			CLCGHAL_API virtual ~GLShaderProgram();

			CLCGHAL_API virtual void dealloc() override;

			CLCGHAL_API static GLShaderProgram *alloc(
				Device *ownerDevice,
				GLuint programHandle
			);

			CLCGHAL_API virtual size_t getUniformBufferIndex(const std::string_view &name) override;
		};
	}
}

#endif
