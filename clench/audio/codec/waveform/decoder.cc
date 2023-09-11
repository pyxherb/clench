#include "decoder.h"

#include <clench/utils/membuf.hh>
#include <cstdint>
#include <sstream>


clench::audio::IWave* clench::audio::WaveFormDecoder::decode(const char* src, std::size_t size) {
	clench::utils::MemoryBuffer mb(src, size);
	{
		char buf[4] = { 0 };

		std::uint32_t szTotal;

		// RIFF chunk
		{
			// Check magic number
			mb.sgetn(buf, 4);
			if (buf[0] != 'R' ||
				buf[1] != 'I' ||
				buf[2] != 'F' ||
				buf[3] != 'F')
				throw DecodeError("Bad RIFF magic number");

			mb.sgetn((char*)&szTotal, sizeof(szTotal));	 // Get total size

			// Check type identifier
			mb.sgetn(buf, 4);
			if (buf[0] != 'W' ||
				buf[1] != 'A' ||
				buf[2] != 'V' ||
				buf[3] != 'E')
				throw DecodeError("Bad RIFF type");
		}

		std::uint16_t fmtAudio, nChannels, szAlign, bps;
		std::uint32_t sampleRate, byteRate;
		// Format chunk
		{
			// Check identifier
			mb.sgetn(buf, 4);
			if (buf[0] != 'f' ||
				buf[1] != 'm' ||
				buf[2] != 't' ||
				buf[3] != 20)
				throw DecodeError("Bad format chunk ID");

			mb.sgetn(buf, 4);  // Chunk size (Ignored)
			if (*(std::uint32_t*)&buf != 16)
				throw DecodeError("Bad format chunk size");
			mb.sgetn((char*)&fmtAudio, 2);	  // Audio format
			mb.sgetn((char*)&nChannels, 2);	  // Channel count
			mb.sgetn((char*)&sampleRate, 4);  // Sample rate
			mb.sgetn((char*)&byteRate, 4);	  // Byte rate
			mb.sgetn((char*)&szAlign, 2);	  // Block alignment
			mb.sgetn((char*)&bps, 2);		  // Bits per sample
		}

		// Data chunk
		{
			// Check identifier
			mb.sgetn(buf, 4);
			if (buf[0] != 'd' ||
				buf[1] != 'a' ||
				buf[2] != 't' ||
				buf[3] != 'a')
				throw DecodeError("Bad data chunk ID");

			std::uint32_t dataSize;
			mb.sgetn((char*)&dataSize, 4);	// Data size in bytes

			char* data = new char[dataSize];
			mb.sgetn(data, dataSize);
		}
	}
}
