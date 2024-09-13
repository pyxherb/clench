#include "texture.h"

using namespace clench;
using namespace clench::ghal;

CLCGHAL_API Texture::Texture(
	GHALDevice *ownedDevice,
	TextureType textureType)
	: GHALDeviceResource(ownedDevice),
	  textureType(textureType) {
}

CLCGHAL_API Texture::~Texture() {
}

CLCGHAL_API Texture1D::Texture1D(
	GHALDevice *ownedDevice,
	const Texture1DDesc &textureDesc)
	: Texture(ownedDevice, TextureType::Texture1D),
	  textureDesc(textureDesc) {
}

CLCGHAL_API Texture1D::~Texture1D() {
}

CLCGHAL_API Texture2D::Texture2D(
	GHALDevice *ownedDevice,
	const Texture2DDesc &textureDesc)
	: Texture(ownedDevice, TextureType::Texture2D),
	  textureDesc(textureDesc) {
}

CLCGHAL_API Texture2D::~Texture2D() {
}

CLCGHAL_API Texture3D::Texture3D(
	GHALDevice *ownedDevice,
	const Texture3DDesc &textureDesc)
	: Texture(ownedDevice, TextureType::Texture2D),
	  textureDesc(textureDesc) {
}

CLCGHAL_API Texture3D::~Texture3D() {
}

#if CLCGHAL_TEXCONV_OPTIMIZED_CODE
extern "C" {
void _clcghalConvertTextureRGB8ToRGBA8(
	const char *src,
	size_t srcSize,
	char *bufferOut);
}
#endif

CLCGHAL_API size_t clench::ghal::getPixelSizeForTextureFormat(TextureFormat textureFormat) {
	switch (textureFormat) {
		case TextureFormat::Gray8:
			return sizeof(uint8_t) * 1;
		case TextureFormat::GrayAlpha8:
			return sizeof(uint8_t) * 2;
		case TextureFormat::Gray16:
			return sizeof(uint16_t) * 1;
		case TextureFormat::GrayAlpha16:
			return sizeof(uint16_t) * 2;
		case TextureFormat::Gray32:
			return sizeof(uint32_t) * 1;
		case TextureFormat::GrayAlpha32:
			return sizeof(uint32_t) * 2;
		case TextureFormat::Gray32F:
			return sizeof(float) * 1;
		case TextureFormat::GrayAlpha32F:
			return sizeof(float) * 2;
		case TextureFormat::RGB8:
			return sizeof(uint8_t) * 3;
		case TextureFormat::RGBA8:
			return sizeof(uint8_t) * 4;
		case TextureFormat::RGB32F:
			return sizeof(float) * 3;
		case TextureFormat::RGBA32F:
			return sizeof(float) * 4;
		case TextureFormat::BGR8:
			return sizeof(uint8_t) * 3;
		case TextureFormat::BGRA8:
			return sizeof(uint8_t) * 4;
		case TextureFormat::BGR32F:
			return sizeof(float) * 3;
		case TextureFormat::BGRA32F:
			return sizeof(float) * 4;
		default:
			throw std::logic_error("Invalid texture format");
	}
}

CLCGHAL_API char *clench::ghal::convertTextureRGB8ToRGBA8(
	const char *src,
	size_t srcSize,
	size_t &sizeOut) {
#ifndef NDEBUG
	if (srcSize % 3)
		throw std::logic_error("Source data are not aligned");
#endif

	sizeOut = srcSize + srcSize / 3;
	std::unique_ptr<char[]> buffer = std::make_unique<char[]>(sizeOut);
#if CLCGHAL_TEXCONV_OPTIMIZED_CODE
	_clcghalConvertTextureRGB8ToRGBA8(src, srcSize, buffer.get());
#else
	for (size_t i = 0, j = 0; i < srcSize; i += 3, j += 4) {
		*((uint16_t *)(buffer.get() + j)) = *(uint16_t *)(src + i);
		*((uint16_t *)(buffer.get() + j + 2)) = ((src[i + 2] << 8) | 0xff);
	}
#endif

	return buffer.release();
}
