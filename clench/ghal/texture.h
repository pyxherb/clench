#ifndef _CLENCH_GHAL_TEXTURE_H_
#define _CLENCH_GHAL_TEXTURE_H_

#include <cstdint>
#include <cstring>
#include <stdexcept>

#include "resource.h"

namespace clench {
	namespace ghal {
		enum class TextureFormat : uint8_t {
			Unknown = 0,  // Unknown

			Gray8,		 // Grayscale, unsigned 8-bit integer
			GrayAlpha8,	 // Grayscale with Alpha, unsigned 8-bit integer

			Gray16,		  // Grayscale, unsigned 16-bit integer
			GrayAlpha16,  // Grayscale with Alpha, unsigned 16-bit integer

			Gray32,		  // Grayscale, unsigned 32-bit integer
			GrayAlpha32,  // Grayscale with Alpha, unsigned 32-bit integer

			Gray32F,	   // Grayscale, unsigned 32-bit floating-point
			GrayAlpha32F,  // Grayscale with Alpha, unsigned 32-bit floating-point

			RGB8,	// RGB, unsigned 8-bit integer
			RGBA8,	// RGBA, unsigned 8-bit integer

			RGB32F,	  // RGB, unsigned 32-bit floating-point
			RGBA32F,  // RGBA, unsigned 32-bit floating-point

			BGR8,	// BGR, unsigned 8-bit integer
			BGRA8,	// BGRA, unsigned 8-bit integer

			BGR32F,	  // BGR, unsigned 32-bit floating-point
			BGRA32F,  // BGRA, unsigned 32-bit floating-point
		};

		enum class TextureType : uint8_t {
			Texture1D = 0,
			Texture2D,
			Texture3D
		};

		enum class TextureUsage {
			Default = 0,
			Static,
			Dynamic,
			Staging
		};

		using TextureBindingFlags = uint32_t;

		constexpr static TextureBindingFlags
			TEXTURE_BINDING_RENDER_TARGET = 0x01,
			TEXTURE_BINDING_SHADER_RESOURCE = 0x02;

		class Texture : public GHALDeviceResource {
		public:
			TextureType textureType;

			CLCGHAL_API Texture(
				GHALDevice *ownedDevice,
				TextureType textureType);
			CLCGHAL_API virtual ~Texture();
		};

		struct Texture1DDesc {
			int width;
			int mipmapLevel;
			TextureFormat format;
			TextureBindingFlags bindingFlags;
		};

		class Texture1D : public Texture {
		public:
			Texture1DDesc textureDesc;

			CLCGHAL_API Texture1D(
				GHALDevice *ownedDevice,
				const Texture1DDesc &textureDesc);
			CLCGHAL_API virtual ~Texture1D();
		};

		struct Texture2DDesc {
			int width, height;
			int mipmapLevel;
			TextureFormat format;
			TextureBindingFlags bindingFlags;
		};

		class Texture2D : public Texture {
		public:
			Texture2DDesc textureDesc;

			CLCGHAL_API Texture2D(
				GHALDevice *ownedDevice,
				const Texture2DDesc &textureDesc);
			CLCGHAL_API virtual ~Texture2D();
		};

		struct Texture3DDesc {
			int width, height, depth;
			int mipmapLevel;
			TextureFormat format;
			TextureBindingFlags bindingFlags;
		};

		class Texture3D : public Texture {
		public:
			Texture3DDesc textureDesc;

			CLCGHAL_API Texture3D(
				GHALDevice *ownedDevice,
				const Texture3DDesc &textureDesc);
			CLCGHAL_API virtual ~Texture3D();
		};

		class TextureFormatError : public std::runtime_error {
		public:
			CLCGHAL_API TextureFormatError(const std::string &s);
			CLCGHAL_API virtual ~TextureFormatError();
		};

		CLCGHAL_API size_t getPixelSizeForTextureFormat(TextureFormat textureFormat);

		CLCGHAL_API char *convertTextureRGB8ToRGBA8(
			const char *src,
			size_t srcSize,
			size_t &sizeOut);
	}
}

#endif
