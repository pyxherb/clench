#include "shader.h"

using namespace clench;
using namespace clench::ghal;

CLCGHAL_API GLVertexShader::GLVertexShader(
	GHALDevice *ownedDevice,
	GLuint shaderHandle)
	: VertexShader(ownedDevice),
	  shaderHandle(shaderHandle) {
}

CLCGHAL_API GLVertexShader::~GLVertexShader() {
	glDeleteShader(shaderHandle);
}

CLCGHAL_API GLFragmentShader::GLFragmentShader(GHALDevice *ownedDevice, GLuint shaderHandle) : FragmentShader(ownedDevice), shaderHandle(shaderHandle) {
}

CLCGHAL_API GLFragmentShader::~GLFragmentShader() {
	glDeleteShader(shaderHandle);
}

CLCGHAL_API GLGeometryShader::GLGeometryShader(GHALDevice *ownedDevice, GLuint shaderHandle) : GeometryShader(ownedDevice), shaderHandle(shaderHandle) {
}

CLCGHAL_API GLGeometryShader::~GLGeometryShader() {
	glDeleteShader(shaderHandle);
}

CLCGHAL_API GLShaderProgram::GLShaderProgram(GHALDevice *ownerDevice, GLuint programHandle) : ShaderProgram(ownerDevice), programHandle(programHandle) {}

CLCGHAL_API GLShaderProgram::~GLShaderProgram() {
	glDeleteProgram(programHandle);
}
