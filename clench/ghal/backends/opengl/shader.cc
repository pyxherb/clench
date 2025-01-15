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

CLCGHAL_API GLVertexShader *GLVertexShader::alloc(
	GHALDevice *ownedDevice,
	GLuint shaderHandle) {
	return (GLVertexShader *)
		peff::allocAndConstruct<GLVertexShader>(
			ownedDevice->resourceAllocator.get(), sizeof(std::max_align_t),
			ownedDevice, shaderHandle);
}

CLCGHAL_API GLFragmentShader::GLFragmentShader(GHALDevice *ownedDevice, GLuint shaderHandle) : FragmentShader(ownedDevice), shaderHandle(shaderHandle) {
}

CLCGHAL_API GLFragmentShader::~GLFragmentShader() {
	glDeleteShader(shaderHandle);
}

CLCGHAL_API GLFragmentShader *GLFragmentShader::alloc(
	GHALDevice *ownedDevice,
	GLuint shaderHandle) {
	return (GLFragmentShader *)
		peff::allocAndConstruct<GLFragmentShader>(
			ownedDevice->resourceAllocator.get(), sizeof(std::max_align_t),
			ownedDevice, shaderHandle);
}

CLCGHAL_API GLGeometryShader::GLGeometryShader(GHALDevice *ownedDevice, GLuint shaderHandle) : GeometryShader(ownedDevice), shaderHandle(shaderHandle) {
}

CLCGHAL_API GLGeometryShader::~GLGeometryShader() {
	glDeleteShader(shaderHandle);
}

CLCGHAL_API GLGeometryShader *GLGeometryShader::alloc(
	GHALDevice *ownedDevice,
	GLuint shaderHandle) {
	return (GLGeometryShader *)
		peff::allocAndConstruct<GLGeometryShader>(
			ownedDevice->resourceAllocator.get(), sizeof(std::max_align_t),
			ownedDevice, shaderHandle);
}

CLCGHAL_API GLShaderProgram::GLShaderProgram(GHALDevice *ownerDevice, GLuint programHandle) : ShaderProgram(ownerDevice), programHandle(programHandle) {}

CLCGHAL_API GLShaderProgram::~GLShaderProgram() {
	glDeleteProgram(programHandle);
}

CLCGHAL_API GLShaderProgram *GLShaderProgram::alloc(
	GHALDevice *ownedDevice,
	GLuint programHandle) {
	return (GLShaderProgram *)
		peff::allocAndConstruct<GLShaderProgram>(
			ownedDevice->resourceAllocator.get(), sizeof(std::max_align_t),
			ownedDevice, programHandle);
}
