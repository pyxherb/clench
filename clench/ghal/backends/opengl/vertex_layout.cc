#include "vertex_layout.h"

using namespace clench;
using namespace clench::ghal;

CLCGHAL_API GLVertexLayout::GLVertexLayout(
	GHALDevice *ownerDevice,
	GLuint vertexArrayHandle)
	: VertexLayout(ownerDevice),
	  vertexArrayHandle(vertexArrayHandle) {
}

CLCGHAL_API GLVertexLayout::~GLVertexLayout() {
	glDeleteVertexArrays(1, &vertexArrayHandle);
}

CLCGHAL_API void GLVertexLayout::dealloc() {
	peff::destroyAndRelease<GLVertexLayout>(ownerDevice->resourceAllocator.get(), this, sizeof(std::max_align_t));
}

CLCGHAL_API GLVertexLayout *GLVertexLayout::alloc(
	GHALDevice *ownerDevice,
	GLuint vertexArrayHandle) {
	return (GLVertexLayout *)
		peff::allocAndConstruct<GLVertexLayout>(
			ownerDevice->resourceAllocator.get(), sizeof(std::max_align_t),
			ownerDevice, vertexArrayHandle);
}
