#include "buffer.h"

using namespace clench;
using namespace clench::ghal;

CLCGHAL_API GLBuffer::GLBuffer(
	GHALDevice *ownerDevice,
	const BufferDesc &bufferDesc,
	GLuint bufferHandle)
	: Buffer(ownerDevice, bufferDesc),
	  bufferHandle(bufferHandle) {
}

CLCGHAL_API GLBuffer::~GLBuffer() {
	glDeleteBuffers(1, &bufferHandle);
}

CLCGHAL_API GLBuffer *GLBuffer::alloc(
	GHALDevice *ownerDevice,
	const BufferDesc &bufferDesc,
	GLuint bufferHandle) {
	return (GLBuffer *)
		peff::allocAndConstruct<GLBuffer>(
			ownerDevice->resourceAllocator.get(), sizeof(std::max_align_t),
			ownerDevice, bufferDesc, bufferHandle);
}
