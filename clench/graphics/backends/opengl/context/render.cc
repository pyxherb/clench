#include "../context.h"

using namespace clench::graphics;

void GLContext::begin() {
}

void GLContext::end() {
}

void GLContext::onResize(Eigen::Vector2i size) {
	glViewport(0, 0, size.x(), size.y());
}

void GLContext::present() {
#ifdef _WIN32
	SwapBuffers(_hdc);
#else
	eglSwapBuffers(_eglDisplay, _eglSurface);
#endif
}

void GLContext::clear(ColorF color) {
	glClearColor(color.r, color.g, color.b, color.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLContext::setVertexLayout(size_t index, VertexLayout layout) {
	GLenum dataType;
	switch (layout.dataType) {
		case VTXDATA_INT:
			dataType = GL_INT;
			break;
		case VTXDATA_UINT:
			dataType = GL_UNSIGNED_INT;
			break;
		case VTXDATA_SHORT:
			dataType = GL_SHORT;
			break;
		case VTXDATA_USHORT:
			dataType = GL_UNSIGNED_SHORT;
			break;
		case VTXDATA_FLOAT:
			dataType = GL_FLOAT;
			break;
		case VTXDATA_DOUBLE:
			dataType = GL_DOUBLE;
			break;
		case VTXDATA_BOOL:
			dataType = GL_BOOL;
			break;
		default:
			throw std::logic_error("Invalid data type");
	}

	glVertexAttribPointer(
		index,
		(GLint)layout.nElements,
		dataType,
		(GLboolean)layout.normalized,
		(GLsizei)layout.stride,
		(void*)layout.off);
}

void GLContext::setVertexLayoutEnabled(size_t index, bool enabled) {
	if (enabled)
		glEnableVertexAttribArray(index);
	else
		glDisableVertexAttribArray(index);
}

void GLContext::setTexture(TextureType type, Texture* texture) {
	GLenum target;

	switch (type) {
		case TextureType::TEXTURE1D:
			target = GL_TEXTURE_1D;
			break;
		case TextureType::TEXTURE2D:
			target = GL_TEXTURE_2D;
			break;
		case TextureType::TEXTURE3D:
			target = GL_TEXTURE_3D;
			break;
		default:
			throw std::logic_error("Invalid texture type");
	}

	glBindTexture(
		target,
		texture ? ((GLTexture*)texture)->handle : GL_NONE);
}

void GLContext::useShaderProgram(ShaderProgram* shaderProgram) {
	glUseProgram(
		shaderProgram
			? ((GLShaderProgram*)shaderProgram)->handle
			: GL_NONE);
}

void GLContext::bindVertexBuffer(VertexBuffer* buf) {
	glBindVertexArray(((GLVertexBuffer*)buf)->vao);
	glBindBuffer(GL_ARRAY_BUFFER, ((GLVertexBuffer*)buf)->vbo);
}
void GLContext::bindVertexBuffer(IndexedVertexBuffer* buf) {
	glBindVertexArray(((GLIndexedVertexBuffer*)buf)->vao);
	glBindBuffer(GL_ARRAY_BUFFER, ((GLIndexedVertexBuffer*)buf)->buffers.vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ((GLIndexedVertexBuffer*)buf)->buffers.ebo);

	_szIndices = ((GLIndexedVertexBuffer*)buf)->szIndices;
}

VertexBuffer* GLContext::createVertexBuffer() {
	return new GLVertexBuffer();
}

IndexedVertexBuffer* GLContext::createIndexedVertexBuffer() {
	return new GLIndexedVertexBuffer();
}

void GLContext::drawIndexedTriangles() {
	glDrawElements(GL_TRIANGLES, _szIndices, GL_UNSIGNED_INT, nullptr);
}

void GLContext::setUniform(size_t index, unsigned int x) {
	glUniform1ui(index, x);
}
