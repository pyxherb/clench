#ifndef _CLENCH_GRAPHICS_CODEC_H_
#define _CLENCH_GRAPHICS_CODEC_H_

#include "base/texture.h"
#include <stdexcept>

namespace clench {
	namespace graphics {
		class ImageDecodeError: std::runtime_error {
		public:
			explicit inline ImageDecodeError(const std::string s):runtime_error(s) {}
			virtual inline ~ImageDecodeError() {};
		};

		class IImageDecoder {
		public:
			IImageDecoder(std::string name);
			virtual ~IImageDecoder() = default;

			virtual void init() = 0;
			virtual void deinit() = 0;
			virtual RawTexture* decode(const char* data, std::size_t len) = 0;
		};

		void initImageDecoders();
		void deinitImageDecoders();
		RawTexture* decodeImage(const char* data, std::size_t len);
	}
}

#endif
