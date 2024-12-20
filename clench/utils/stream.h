#ifndef _CLENCH_STREAM_H_
#define _CLENCH_STREAM_H_

#include "basedefs.h"
#include <cstddef>
#include <cstdint>

namespace clench {
	namespace utils {
		enum StreamSeekMode {
			Set = 0,
			Current,
			End
		};

		constexpr static size_t
			INVALID_STREAM_POS = SIZE_MAX;

		class Stream {
		public:
			CLENCH_NO_COPY_MOVE_METHODS(Stream);

			CLCUTILS_API Stream();
			CLCUTILS_API virtual ~Stream();

			virtual bool seek(StreamSeekMode mode, long long offset) = 0;
			virtual size_t tell() = 0;
		};

		class InputStream : virtual public Stream {
		public:
			CLENCH_NO_COPY_MOVE_METHODS(InputStream);

			CLCUTILS_API InputStream();
			CLCUTILS_API ~InputStream();

			virtual size_t read(void *buffer, size_t size) = 0;
		};

		class OutputStream : virtual public Stream {
		public:
			CLENCH_NO_COPY_MOVE_METHODS(OutputStream);

			CLCUTILS_API OutputStream();
			CLCUTILS_API ~OutputStream();

			virtual size_t write(const void *buffer, size_t size) = 0;
		};

		class InputOutputStream : public InputStream, public OutputStream {
		public:
			CLENCH_NO_COPY_MOVE_METHODS(InputOutputStream);

			CLCUTILS_API InputOutputStream();
			CLCUTILS_API ~InputOutputStream();
		};
	}
}

#endif
