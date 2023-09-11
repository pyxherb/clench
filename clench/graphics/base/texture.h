#ifndef _CLENCH_GRAPHICS_TEXTURE_H_
#define _CLENCH_GRAPHICS_TEXTURE_H_

#include <clench/math.h>
#include <cstring>

namespace clench {
	namespace graphics {
		enum class TextureFormat : std::uint8_t {
			UNKNOWN = 0,  // Unknown

			MONO,  // Monochrome (Black/White), 1 bit

			GRAY8,		   // Grayscale, unsigned 8-bit integer
			GRAYALPHA8,	   // Grayscale with alpha, unsigned 8-bit integer
			GRAY16,		   // Grayscale, unsigned 16-bit integer
			GRAYALPHA16,   // Grayscale with alpha, unsigned 16-bit integer
			GRAY32,		   // Grayscale, unsigned 32-bit integer
			GRAYALPHA32,   // Grayscale with alpha, unsigned 32-bit integer
			GRAY32F,	   // Grayscale, unsigned 32-bit floating-point
			GRAYALPHA32F,  // Grayscale with alpha, unsigned 32-bit floating-point

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

		enum class TextureType : std::uint8_t {
			TEXTURE1D,
			TEXTURE2D,
			TEXTURE3D
		};

		using TextureBakeFlag = std::uint8_t;

		class RawTexture {
		public:
			virtual inline ~RawTexture(){};

			virtual void setData(
				const char* src,
				std::size_t dataSize,
				TextureFormat format,
				Eigen::Vector2i textureSize) = 0;

			virtual void* getData() = 0;
			virtual std::size_t getDataSize() = 0;
			virtual TextureFormat getFormat() = 0;
			virtual Eigen::Vector2i getTextureSize() = 0;
		};

		class MemoryTexture : public RawTexture {
		protected:
			char* _data = nullptr;
			std::size_t _dataSize = 0;
			TextureFormat _format = TextureFormat::UNKNOWN;
			Eigen::Vector2i _textureSize;

		public:
			virtual inline void setData(
				const char* src,
				std::size_t dataSize,
				TextureFormat format,
				Eigen::Vector2i textureSize) override {
				if (_data)
					delete[] _data;
				if (dataSize != _dataSize)
					_data = new char[dataSize];

				if (src) {
					assert(_data);
					memcpy(_data, src, dataSize);
				}

				_dataSize = dataSize;
				_format = format;
				_textureSize = textureSize;
			}

			virtual inline void* getData() override { return _data; }
			virtual inline std::size_t getDataSize() override { return _dataSize; }
			virtual inline TextureFormat getFormat() override { return _format; }
			virtual Eigen::Vector2i getTextureSize() override { return _textureSize; }

			inline MemoryTexture(
				const char* src,
				std::size_t dataSize,
				TextureFormat format,
				Eigen::Vector2i textureSize) {
				setData(src, dataSize, format, textureSize);
			}
			virtual inline ~MemoryTexture() {
				if (_data)
					delete[] _data;
			}
		};

		class TextureFormatError : public std::runtime_error {
		public:
			explicit inline TextureFormatError(std::string s) : runtime_error(s) {}
			virtual inline ~TextureFormatError(){};
		};

		class Texture {
		public:
			virtual inline ~Texture() {}
		};
	}
}

#endif
