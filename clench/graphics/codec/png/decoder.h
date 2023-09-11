#ifndef _CLENCH_GRAPHICS_CODEC_PNG_DECODER_H_
#define _CLENCH_GRAPHICS_CODEC_PNG_DECODER_H_

#include <clench/graphics/codec.h>

namespace clench {
	namespace graphics {
		class PNGImageDecoder : public IImageDecoder {
		public:
			PNGImageDecoder();
			virtual ~PNGImageDecoder() = default;

			virtual void init() override;
			virtual void deinit() override;

			virtual RawTexture* decode(const char* data, size_t len) override;
		};
	}
}

#endif
