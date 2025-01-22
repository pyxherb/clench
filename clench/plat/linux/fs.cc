#include "../fs.h"

using namespace clench;
using namespace clench::plat;

CLCPLAT_API NativeFileStream::NativeFileStream(int fd) : fd(fd) {
}

CLCPLAT_API NativeFileStream::~NativeFileStream() {
	close();
}

CLCPLAT_API bool NativeFileStream::seek(utils::StreamSeekMode mode, long long offset) {
	int whence;

	switch (mode) {
		case utils::StreamSeekMode::Set:
			whence = SEEK_SET;
			break;
		case utils::StreamSeekMode::Current:
			whence = SEEK_CUR;
			break;
		case utils::StreamSeekMode::End:
			whence = SEEK_END;
			break;
	}

	if(lseek64(fd, offset, whence) < 0)
		return false;

	return true;
}

CLCPLAT_API size_t NativeFileStream::tell() {
	if (long offset = lseek64(fd, SEEK_CUR, 0); offset > -1)
		return (size_t)offset;
	return utils::INVALID_STREAM_POS;
}

CLCPLAT_API size_t NativeFileStream::read(void *buffer, size_t size) {
	ssize_t readSize = ::read(fd, buffer, size);

	if (readSize < 0)
		return 0;

	return (size_t)readSize;
}

CLCPLAT_API size_t NativeFileStream::write(const void *buffer, size_t size) {
	ssize_t writtenSize = ::write(fd, buffer, size);

	if (writtenSize < 0)
		return 0;

	return (size_t)writtenSize;
}

CLCPLAT_API void NativeFileStream::close() {
	if(!(fd < 0))
		::close(fd);
}

CLCPLAT_API FileStream *NativeFileSystem::openFile(const char *path, FileOpenMode mode) {
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
