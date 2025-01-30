#include "texture.h"

using namespace clench;
using namespace clench::ghal;

CLCGHAL_API GLTexture1D::GLTexture1D(
	Device *ownerDevice,
	const Texture1DDesc &textureDesc,
	GLuint textureHandle)
	: Texture1D(ownerDevice, textureDesc),
	  textureHandle(textureHandle) {
}

CLCGHAL_API GLTexture1D::~GLTexture1D() {
	glDeleteTextures(1, &textureHandle);
}

CLCGHAL_API void GLTexture1D::dealloc() {
	peff::destroyAndRelease<GLTexture1D>(ownerDevice->resourceAllocator.get(), this, sizeof(std::max_align_t));
}

CLCGHAL_API GLTexture1D *GLTexture1D::alloc(
	Device *ownedDevice,
	const Texture1DDesc &textureDesc,
	GLuint textureHandle) {
	return peff::allocAndConstruct<GLTexture1D>(
		ownedDevice->resourceAllocator.get(), sizeof(std::max_align_t),
		ownedDevice,
		textureDesc,
		textureHandle);
}

CLCGHAL_API GLTexture2D::GLTexture2D(
	Device *ownerDevice,
	const Texture2DDesc &textureDesc,
	GLuint textureHandle)
	: Texture2D(ownerDevice, textureDesc),
	  textureHandle(textureHandle) {
}

CLCGHAL_API GLTexture2D::~GLTexture2D() {
	glDeleteTextures(1, &textureHandle);
}

CLCGHAL_API void GLTexture2D::dealloc() {
	peff::destroyAndRelease<GLTexture2D>(ownerDevice->resourceAllocator.get(), this, sizeof(std::max_align_t));
}

CLCGHAL_API GLTexture2D *GLTexture2D::alloc(
	Device *ownedDevice,
	const Texture2DDesc &textureDesc,
	GLuint textureHandle) {
	return peff::allocAndConstruct<GLTexture2D>(
		ownedDevice->resourceAllocator.get(), sizeof(std::max_align_t),
		ownedDevice,
		textureDesc,
		textureHandle);
}

CLCGHAL_API GLTexture3D::GLTexture3D(
	Device *ownerDevice,
	const Texture3DDesc &textureDesc,
	GLuint textureHandle)
	: Texture3D(ownerDevice, textureDesc),
	  textureHandle(textureHandle) {
}

CLCGHAL_API GLTexture3D::~GLTexture3D() {
	glDeleteTextures(1, &textureHandle);
}

CLCGHAL_API void GLTexture3D::dealloc() {
	peff::destroyAndRelease<GLTexture3D>(ownerDevice->resourceAllocator.get(), this, sizeof(std::max_align_t));
}

CLCGHAL_API GLTexture3D *GLTexture3D::alloc(
	Device *ownedDevice,
	const Texture3DDesc &textureDesc,
	GLuint textureHandle) {
	return peff::allocAndConstruct<GLTexture3D>(
		ownedDevice->resourceAllocator.get(), sizeof(std::max_align_t),
		ownedDevice,
		textureDesc,
		textureHandle);
}
