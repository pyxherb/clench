#ifndef _CLENCH_TCM_DECODERS_PNG_DECODER_H_
#define _CLENCH_TCM_DECODERS_PNG_DECODER_H_

#include <clench/tcm/decoder.h>

namespace clench {
	namespace tcm {
		class PNGDecoder : public Decoder {
		public:
			CLCTCM_API ~PNGDecoder();

			CLCTCM_API virtual void *decode(
				const char *data,
				size_t size,
				size_t &sizeOut,
				int &widthOut,
				int &heightOut,
				ghal::TextureFormat &textureFormatOut) override;
		};
	}
}

#endif
