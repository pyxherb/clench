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
	void *glBuffer = (GLBuffer*)ownerDevice->resourceAllocator->alloc(sizeof(GLBuffer));
	if(!glBuffer)
		return nullptr;

	new (glBuffer) GLBuffer(ownerDevice, bufferDesc, bufferHandle);

	return (GLBuffer*)glBuffer;
}
