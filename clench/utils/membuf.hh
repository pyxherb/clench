#ifndef _CLENCH_UTILS_MSTREAM_HH_
#define _CLENCH_UTILS_MSTREAM_HH_

#include <streambuf>

namespace clench {
	namespace utils {
		class MemoryBuffer : public std::streambuf {
		public:
			template<typename BufferT>
			MemoryBuffer(const BufferT* buf, std::size_t size = SIZE_MAX) {
				setbuf((char_type*)buf,size);
			}
			virtual ~MemoryBuffer() {}
		};
	}
}

#endif
