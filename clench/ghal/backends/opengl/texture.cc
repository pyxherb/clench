#include "texture.h"

using namespace clench;
using namespace clench::ghal;

CLCGHAL_API GLTexture1D::GLTexture1D(
	GHALDevice *ownerDevice,
	const Texture1DDesc &textureDesc,
	GLuint textureHandle)
	: Texture1D(ownerDevice, textureDesc),
	textureHandle(textureHandle) {
}

CLCGHAL_API GLTexture1D::~GLTexture1D() {
	glDeleteTextures(1, &textureHandle);
}

CLCGHAL_API GLTexture2D::GLTexture2D(
	GHALDevice *ownerDevice,
	const Texture2DDesc &textureDesc,
	GLuint textureHandle)
	: Texture2D(ownerDevice, textureDesc),
	textureHandle(textureHandle) {
}

CLCGHAL_API GLTexture2D::~GLTexture2D() {
	glDeleteTextures(1, &textureHandle);
}

CLCGHAL_API GLTexture3D::GLTexture3D(
	GHALDevice *ownerDevice,
	const Texture3DDesc &textureDesc,
	GLuint textureHandle)
	: Texture3D(ownerDevice, textureDesc),
	textureHandle(textureHandle) {
}

CLCGHAL_API GLTexture3D::~GLTexture3D() {
	glDeleteTextures(1, &textureHandle);
}
