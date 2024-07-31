#ifndef _CLENCH_RHI_TEXTURE_H_
#define _CLENCH_RHI_TEXTURE_H_

#include <cstdint>
#include <cstring>
#include <stdexcept>

#include "resource.h"

namespace clench {
	namespace rhi {
		enum class TextureFormat : uint8_t {
			Unknown = 0,  // Unknown

			Mono,  // Monochrome (Black/White), 1 bit

			Gray8,		   // Grayscale, unsigned 8-bit integer
			GrayAlpha8,	   // Grayscale with Alpha, unsigned 8-bit integer
			Gray16,		   // Grayscale, unsigned 16-bit integer
			GrayAlpha16,   // Grayscale with Alpha, unsigned 16-bit integer
			Gray32,		   // Grayscale, unsigned 32-bit integer
			GrayAlpha32,   // Grayscale with Alpha, unsigned 32-bit integer
			Gray32F,	   // Grayscale, unsigned 32-bit floating-point
			GrayAlpha32F,  // Grayscale with Alpha, unsigned 32-bit floating-point

			RGB8,	  // RGB, unsigned 8-bit integer
			RGBA8,	  // RGBA, unsigned 8-bit integer
			ARGB8,	  // ARGB, unsigned 8-bit integer
			RGB32F,	  // RGB, unsigned 32-bit floating-point
			RGBA32F,  // RGBA, unsigned 32-bit floating-point
			ARGB32F,  // ARGB, unsigned 32-bit floating-point

			BGR8,	  // BGR, unsigned 8-bit integer
			BGRA8,	  // BGRA, unsigned 8-bit integer
			ABGR8,	  // ABGR, unsigned 8-bit integer
			BGR32F,	  // BGR, unsigned 32-bit floating-point
			BGRA32F,  // BGRA, unsigned 32-bit floating-point
			ABGR32F	  // ABGR, unsigned 32-bit floating-point
		};

		enum class TextureType : uint8_t {
			Texture1D = 0,
			Texture2D,
			Texture3D
		};

		class Texture : public RHIDeviceResource {
		public:
			TextureType textureType;

			CLCRHI_API Texture(
				RHIDevice* ownedDevice,
				TextureType textureType);
			CLCRHI_API virtual ~Texture();
		};

		struct Texture1DDesc {
			int width;
			int mipmapLevel;
			TextureFormat format;
		};

		class Texture1D : public Texture {
		public:
			Texture1DDesc textureDesc;

			CLCRHI_API Texture1D(
				RHIDevice *ownedDevice,
				const Texture1DDesc &textureDesc);
			CLCRHI_API virtual ~Texture1D();
		};

		struct Texture2DDesc {
			int width, height;
			int mipmapLevel;
			TextureFormat format;
		};

		class Texture2D : public Texture {
		public:
			Texture2DDesc textureDesc;

			CLCRHI_API Texture2D(
				RHIDevice* ownedDevice,
				const Texture2DDesc &textureDesc);
			CLCRHI_API virtual ~Texture2D();
		};

		struct Texture3DDesc {
			int width, height, depth;
			int mipmapLevel;
			TextureFormat format;
		};

		class Texture3D : public Texture {
		public:
			Texture3DDesc textureDesc;

			CLCRHI_API Texture3D(
				RHIDevice* ownedDevice,
				const Texture3DDesc &textureDesc);
			CLCRHI_API virtual ~Texture3D();
		};

		class TextureFormatError : public std::runtime_error {
		public:
			CLCRHI_API TextureFormatError(const std::string& s);
			CLCRHI_API virtual ~TextureFormatError();
		};
	}
}

#endif
