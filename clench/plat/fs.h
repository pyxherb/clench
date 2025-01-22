#ifndef _CLENCH_PLAT_FS_H_
#define _CLENCH_PLAT_FS_H_

#include "basedefs.h"
#include <clench/utils/stream.h>
#include <cstdint>

#if defined(_WIN32)

	#include <Windows.h>

#elif __linux__

	#include <fcntl.h>
	#include <unistd.h>

#endif

namespace clench {
	namespace plat {
		class Path {
		public:
		};

		using FileOpenMode = uint32_t;

		constexpr static FileOpenMode
			OPENFILE_READ = 0x0001,
			OPENFILE_WRITE = 0x0002,
			OPENFILE_CREATE = 0x1000,
			OPENFILE_APPEND = 0x2000,
			OPENFILE_BINARY = 0x4000,
			OPENFILE_TRUNCATE = 0x8000;

		class FileStream : public utils::InputOutputStream {
		public:
			CLCPLAT_API FileStream();
			CLCPLAT_API virtual ~FileStream();

			virtual void close() = 0;
		};

		class FileSystem {
		public:
			CLCPLAT_API FileSystem();
			CLCPLAT_API virtual ~FileSystem();

			virtual FileStream *openFile(const char *path, FileOpenMode mode) = 0;
			virtual FileStream *createFile(const char *path, FileOpenMode mode) = 0;
		};

		class NativeFileStream : public FileStream {
		public:
#ifdef _WIN32
			HANDLE handle;

			CLCPLAT_API NativeFileStream(HANDLE handle);
#else
			int fd;

			CLCPLAT_API NativeFileStream(int fd);
#endif
			CLCPLAT_API virtual ~NativeFileStream();

			CLCPLAT_API virtual bool seek(utils::StreamSeekMode mode, long long offset) override;
			CLCPLAT_API virtual size_t tell() override;
			CLCPLAT_API virtual size_t read(void *buffer, size_t size) override;
			CLCPLAT_API virtual size_t write(const void *buffer, size_t size) override;
			CLCPLAT_API virtual void close() override;
		};

		class NativeFileSystem : public FileSystem {
		public:
			CLCPLAT_API NativeFileSystem();
			CLCPLAT_API virtual ~NativeFileSystem();

			CLCPLAT_API virtual FileStream *openFile(const char *path, FileOpenMode mode) override;
		};
	}
}

#endif
