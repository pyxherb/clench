#include "vertex_layout.h"

using namespace clench;
using namespace clench::ghal;

CLCGHAL_API GLVertexLayout::GLVertexLayout(
	Device *ownerDevice)
	: VertexLayout(ownerDevice),
	dataInputs(ownerDevice->resourceAllocator.get()) {
}

CLCGHAL_API GLVertexLayout::~GLVertexLayout() {
}

CLCGHAL_API void GLVertexLayout::dealloc() {
	peff::destroyAndRelease<GLVertexLayout>(ownerDevice->resourceAllocator.get(), this, sizeof(std::max_align_t));
}

CLCGHAL_API GLVertexLayout *GLVertexLayout::alloc(
	Device *ownerDevice) {
	return (GLVertexLayout *)
		peff::allocAndConstruct<GLVertexLayout>(
			ownerDevice->resourceAllocator.get(), sizeof(std::max_align_t),
			ownerDevice);
}
