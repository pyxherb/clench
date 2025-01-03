#ifndef _CLENCH_UTILS_BITARRAY_H_
#define _CLENCH_UTILS_BITARRAY_H_

#include "basedefs.h"
#include <cstddef>
#include <cstdint>
#include <cstring>

namespace clench {
	namespace utils {
		class BitArray {
		private:
			size_t _nBits = 0;
			uint8_t *_buffer = nullptr;

		public:
			CLENCH_FORCEINLINE size_t size() {
				return (_nBits + 7) >> 3;
			}

			CLENCH_FORCEINLINE size_t bitSize() {
				return _nBits;
			}

			CLENCH_FORCEINLINE void resize(size_t nBits) {
				const size_t nBytes = (nBits + 7) >> 3;

				uint8_t *newBuffer = new uint8_t[nBytes];

				if (_buffer) {
					memcpy((void *)newBuffer, (void *)_buffer, nBytes);
					newBuffer[nBytes - 1] &= ~(0xff >> (nBits & 7));
					delete[] _buffer;
				} else {
					memset((void *)newBuffer, 0, nBytes);
				}

				_buffer = newBuffer;
				_nBits = nBits;
			}

			CLENCH_FORCEINLINE BitArray(size_t nBits = 0) {
				resize(nBits);
			}

			CLENCH_FORCEINLINE uint8_t *data() {
				return _buffer;
			}

			CLENCH_FORCEINLINE void setBit(size_t bitIndex) {
				assert(bitIndex < _nBits);

				_buffer[(bitIndex >> 3)] |= (1 << (bitIndex & 7));
			}

			CLENCH_FORCEINLINE void clearBit(size_t bitIndex) {
				assert(bitIndex < _nBits);

				_buffer[(bitIndex >> 3)] &= ~(1 << (bitIndex & 7));
			}

			CLENCH_FORCEINLINE bool getBit(size_t bitIndex) {
				return (_buffer[(bitIndex >> 3)] >> bitIndex) & 1;
			}

			inline void set(size_t bitIndex, uint8_t bits, size_t nBits) {
				assert(bitIndex < _nBits);

				size_t index = (bitIndex >> 3), inBitIndex = (bitIndex & 7);

				_buffer[index] &= (0xff << inBitIndex);
				_buffer[index] |= (bits << inBitIndex);

				size_t tailSize = inBitIndex + nBits - 8;

				_buffer[index + 1] &= (0xff >> (8 - tailSize));
				_buffer[index + 1] |= (bits >> (8 - tailSize));
			}
		};
	}
}

#endif
