#include "views.h"

using namespace clench;
using namespace clench::ghal;

CLCGHAL_API GLRenderTargetView::GLRenderTargetView(
	Device *ownerDevice,
	RenderTargetViewType type,
	GLuint frameBufferHandle)
	: RenderTargetView(ownerDevice, type),
	  frameBufferHandle(frameBufferHandle) {
}

CLCGHAL_API GLRenderTargetView::~GLRenderTargetView() {
}

CLCGHAL_API void GLRenderTargetView::dealloc() {
	peff::destroyAndRelease<GLRenderTargetView>(ownerDevice->resourceAllocator.get(), this, sizeof(std::max_align_t));
}

CLCGHAL_API GLRenderTargetView *GLRenderTargetView::alloc(
	Device *ownerDevice,
	RenderTargetViewType type,
	GLuint frameBufferHandle) {
	return peff::allocAndConstruct<GLRenderTargetView>(
		ownerDevice->resourceAllocator.get(), sizeof(std::max_align_t),
		ownerDevice, type, frameBufferHandle);
}

CLCGHAL_API GLDepthStencilView::GLDepthStencilView(
	Device *ownerDevice,
	DepthStencilViewType type,
	GLuint frameBufferHandle,
	GLuint depthBufferHandle,
	GLuint stencilBufferHandle)
	: DepthStencilView(ownerDevice, type),
	  frameBufferHandle(frameBufferHandle),
	  depthBufferHandle(depthBufferHandle),
	  stencilBufferHandle(stencilBufferHandle) {
}

CLCGHAL_API GLDepthStencilView::~GLDepthStencilView() {
}

CLCGHAL_API void GLDepthStencilView::dealloc() {
	peff::destroyAndRelease<GLDepthStencilView>(ownerDevice->resourceAllocator.get(), this, sizeof(std::max_align_t));
}

CLCGHAL_API GLDepthStencilView *GLDepthStencilView::alloc(
	Device *ownerDevice,
	DepthStencilViewType type,
	GLuint frameBufferHandle,
	GLuint depthBufferHandle,
	GLuint stencilBufferHandle
) {
	return peff::allocAndConstruct<GLDepthStencilView>(
		ownerDevice->resourceAllocator.get(), sizeof(std::max_align_t),
		ownerDevice, type, frameBufferHandle, depthBufferHandle, stencilBufferHandle
	);
}
