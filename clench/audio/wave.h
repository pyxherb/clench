#ifndef _CLENCH_AUDIO_WAVE_H_
#define _CLENCH_AUDIO_WAVE_H_

#include <cstddef>
#include <cstdint>

namespace clench {
	namespace audio {
		enum class AudioFormat:std::uint8_t {

		};

		struct WaveFormat {
			AudioFormat fmt;
			std::uint8_t nChannel, szAlign, bitsPerSec;
		};

		class IWave {
		public:
			inline ~IWave(){};

			virtual void setBuffer(const char* src, std::size_t size) = 0;
			virtual void setBufferShared(const char* src, std::size_t size) = 0;

			virtual const char* getBuffer() = 0;
			virtual std::size_t getSize() = 0;

			virtual bool isBufferShared() = 0;

			virtual WaveFormat getFormat() = 0;
		};
	}
}

#endif
