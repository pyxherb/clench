#include "shader.h"

using namespace clench;
using namespace clench::ghal;

CLCGHAL_API GLVertexShader::GLVertexShader(
	Device *ownedDevice,
	GLuint shaderHandle)
	: VertexShader(ownedDevice),
	  shaderHandle(shaderHandle) {
}

CLCGHAL_API GLVertexShader::~GLVertexShader() {
	glDeleteShader(shaderHandle);
}

CLCGHAL_API void GLVertexShader::dealloc() {
	peff::destroyAndRelease<GLVertexShader>(ownerDevice->resourceAllocator.get(), this, sizeof(std::max_align_t));
}

CLCGHAL_API GLVertexShader *GLVertexShader::alloc(
	Device *ownedDevice,
	GLuint shaderHandle) {
	return peff::allocAndConstruct<GLVertexShader>(
		ownedDevice->resourceAllocator.get(), sizeof(std::max_align_t),
		ownedDevice, shaderHandle);
}

CLCGHAL_API GLFragmentShader::GLFragmentShader(Device *ownedDevice, GLuint shaderHandle) : FragmentShader(ownedDevice), shaderHandle(shaderHandle) {
}

CLCGHAL_API GLFragmentShader::~GLFragmentShader() {
	glDeleteShader(shaderHandle);
}

CLCGHAL_API void GLFragmentShader::dealloc() {
	peff::destroyAndRelease<GLFragmentShader>(ownerDevice->resourceAllocator.get(), this, sizeof(std::max_align_t));
}

CLCGHAL_API GLFragmentShader *GLFragmentShader::alloc(
	Device *ownedDevice,
	GLuint shaderHandle) {
	return peff::allocAndConstruct<GLFragmentShader>(
		ownedDevice->resourceAllocator.get(), sizeof(std::max_align_t),
		ownedDevice, shaderHandle);
}

CLCGHAL_API GLGeometryShader::GLGeometryShader(Device *ownedDevice, GLuint shaderHandle) : GeometryShader(ownedDevice), shaderHandle(shaderHandle) {
}

CLCGHAL_API GLGeometryShader::~GLGeometryShader() {
	glDeleteShader(shaderHandle);
}

CLCGHAL_API void GLGeometryShader::dealloc() {
	peff::destroyAndRelease<GLGeometryShader>(ownerDevice->resourceAllocator.get(), this, sizeof(std::max_align_t));
}

CLCGHAL_API GLGeometryShader *GLGeometryShader::alloc(
	Device *ownedDevice,
	GLuint shaderHandle) {
	return (GLGeometryShader *)
		peff::allocAndConstruct<GLGeometryShader>(
			ownedDevice->resourceAllocator.get(), sizeof(std::max_align_t),
			ownedDevice, shaderHandle);
}

CLCGHAL_API GLShaderProgram::GLShaderProgram(Device *ownerDevice, GLuint programHandle) : ShaderProgram(ownerDevice), programHandle(programHandle) {}

CLCGHAL_API GLShaderProgram::~GLShaderProgram() {
	glDeleteProgram(programHandle);
}

CLCGHAL_API void GLShaderProgram::dealloc() {
	peff::destroyAndRelease<GLShaderProgram>(ownerDevice->resourceAllocator.get(), this, sizeof(std::max_align_t));
}

CLCGHAL_API size_t GLShaderProgram::getUniformBufferIndex(const std::string_view &name) {
	GLuint index = glGetUniformBlockIndex(programHandle, name.data());

	if(index == UINT_MAX)
		return SIZE_MAX;

	return index;
}

CLCGHAL_API GLShaderProgram *GLShaderProgram::alloc(
	Device *ownedDevice,
	GLuint programHandle) {
	return (GLShaderProgram *)
		peff::allocAndConstruct<GLShaderProgram>(
			ownedDevice->resourceAllocator.get(), sizeof(std::max_align_t),
			ownedDevice, programHandle);
}
