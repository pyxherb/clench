#ifndef _CLENCH_TCM_ECODER_H_
#define _CLENCH_TCM_DECODER_H_

#include "basedefs.h"
#include <unordered_map>
#include <clench/ghal/texture.h>

namespace clench {
	namespace tcm {
		class Decoder {
		public:
			CLCTCM_API ~Decoder();

			virtual void *decode(
				const char *data,
				size_t size,
				size_t &sizeOut,
				int &widthOut,
				int &heightOut,
				ghal::TextureFormat &textureFormatOut) = 0;
		};

		CLCTCM_API extern std::unordered_map<std::string, std::unique_ptr<Decoder>> g_registeredDecoders;

		CLCTCM_API void registerDecoder(const std::string &name, Decoder *decoder);
		CLCTCM_API void unregisterDecoder(const std::string &name);

		CLCTCM_API void registerBuiltinDecoders();
	}
}

#endif
