#ifndef _CLENCH_AUDIO_CODEC_H_
#define _CLENCH_AUDIO_CODEC_H_

#include "wave.h"
#include <memory>
#include <stdexcept>

namespace clench {
	namespace audio {
		class DecodeError: std::runtime_error {
		public:
			explicit inline DecodeError(const std::string s):runtime_error(s) {}
			virtual inline ~DecodeError() {};
		};

		class IAudioDecoder {
		public:
			virtual inline ~IAudioDecoder(){};

			virtual IWave* decode(const char* src) = 0;
		};
	}
}

#endif
