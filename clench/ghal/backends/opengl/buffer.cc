#include "buffer.h"

using namespace clench;
using namespace clench::ghal;

CLCGHAL_API GLBuffer::GLBuffer(
	Device *ownerDevice,
	const BufferDesc &bufferDesc,
	GLuint bufferHandle)
	: Buffer(ownerDevice, bufferDesc),
	  bufferHandle(bufferHandle) {
}

CLCGHAL_API GLBuffer::~GLBuffer() {
	glDeleteBuffers(1, &bufferHandle);
}

CLCGHAL_API void GLBuffer::dealloc() {
	peff::destroyAndRelease<GLBuffer>(ownerDevice->resourceAllocator.get(), this, sizeof(std::max_align_t));
}

CLCGHAL_API GLBuffer *GLBuffer::alloc(
	Device *ownerDevice,
	const BufferDesc &bufferDesc,
	GLuint bufferHandle) {
	return (GLBuffer *)
		peff::allocAndConstruct<GLBuffer>(
			ownerDevice->resourceAllocator.get(), sizeof(std::max_align_t),
			ownerDevice, bufferDesc, bufferHandle);
}
