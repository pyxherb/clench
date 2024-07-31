#include "texture.h"

using namespace clench;
using namespace clench::rhi;

CLCRHI_API Texture::Texture(
	RHIDevice* ownedDevice,
	TextureType textureType)
	:RHIDeviceResource(ownedDevice),
	textureType(textureType) {
}

CLCRHI_API Texture::~Texture() {
}

CLCRHI_API Texture1D::Texture1D(
	RHIDevice* ownedDevice,
	const Texture1DDesc& textureDesc)
	:Texture(ownedDevice, TextureType::Texture1D),
	textureDesc(textureDesc) {
}

CLCRHI_API Texture1D::~Texture1D() {
}

CLCRHI_API Texture2D::Texture2D(
	RHIDevice* ownedDevice,
	const Texture2DDesc& textureDesc)
	:Texture(ownedDevice, TextureType::Texture2D),
	textureDesc(textureDesc) {
}

CLCRHI_API Texture2D::~Texture2D() {
}

CLCRHI_API Texture3D::Texture3D(
	RHIDevice* ownedDevice,
	const Texture3DDesc& textureDesc)
	:Texture(ownedDevice, TextureType::Texture2D),
	textureDesc(textureDesc) {
}

CLCRHI_API Texture3D::~Texture3D() {
}