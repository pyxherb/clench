#ifndef _CLENCH_AUDIO_CODEC_WAVFORM_DECODER_H_
#define _CLENCH_AUDIO_CODEC_WAVFORM_DECODER_H_

#include <clench/audio/codec.h>

namespace clench {
	namespace audio {
		class WaveFormDecoder : public clench::audio::IAudioDecoder {
		public:
			virtual IWave* decode(const char* src, std::size_t size) = 0;
		};
	}
}

#endif
