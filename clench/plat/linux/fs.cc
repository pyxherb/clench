#include "../fs.h"

using namespace clench;
using namespace clench::plat;

NativeFileStream::NativeFileStream(int fd) : fd(fd) {
}

CLCUTILS_API bool NativeFileStream::seek(StreamSeekMode mode, long long offset) {
	int whence;

	switch (mode) {
		case StreamSeekMode::Set:
			whence = SEEK_SET;
			break;
		case StreamSeekMode::Current:
			whence = SEEK_CUR;
			break;
		case StreamSeekMode::End:
			whence = SEEK_END;
			break;
	}

	if(lseek64(fd, offset, whence) < 0)
		return false;

	return true;
}

CLCUTILS_API size_t NativeFileStream::tell() {
	if (long offset = lseek64(fd, SEEK_CUR, 0); offset > -1)
		return (size_t)offset;
	return INVALID_STREAM_POS;
}

CLCUTILS_API size_t NativeFileStream::read(void *buffer, size_t size) {
	ssize_t size = ::read(fd, buffer, size);

	if (size < 0)
		return 0;

	return (size_t)size;
}

CLCUTILS_API size_t NativeFileStream::write(const void *buffer, size_t size) {
	ssize_t size = ::write(fd, buffer, size);

	if (size < 0)
		return 0;

	return (size_t)size;
}

CLCUTILS_API FileStream *NativeFileSystem::openFile(const char *path, FileOpenMode mode) {
	int openFlags;

	if (mode & OPENFILE_WRITE) {
		if (mode & OPENFILE_READ)
			openFlags = O_RDWR;
		else
			openFlags = O_WRONLY;
	} else
		openFlags = O_RDONLY;

	if (mode & OPENFILE_TRUNCATE)
		openFlags |= O_TRUNC;

	if (mode & OPENFILE_APPEND)
		openFlags |= O_APPEND;

	if (mode & OPENFILE_CREATE)
		openFlags |= O_CREAT;

	int fd = open(path, openFlags);
	if (fd < 0)
		return nullptr;

	return new NativeFileStream(fd);
}
