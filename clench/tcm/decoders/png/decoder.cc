#include "decoder.h"

#include <png.h>

using namespace clench;
using namespace clench::tcm;

CLCTCM_API PNGDecoder::~PNGDecoder() {}

struct PNGDecodeIoContext {
	const char *data;
	size_t size;
	size_t offset;
};

CLCTCM_API void *PNGDecoder::decode(
	const char *data,
	size_t size,
	size_t &sizeOut,
	int &widthOut,
	int &heightOut,
	ghal::TextureFormat &textureFormatOut) {
	png_structp ps = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
	png_infop pi = png_create_info_struct(ps);
	std::unique_ptr<char[]> texData;

	if (setjmp(png_jmpbuf(ps))) {
		if (texData)
			texData.reset();
		png_destroy_read_struct(&ps, &pi, nullptr);
		return nullptr;
	}

	PNGDecodeIoContext ioContext;

	png_set_read_fn(ps, &ioContext, [](png_structp ps, png_bytep dest, size_t size) {
		PNGDecodeIoContext *ioContext = (PNGDecodeIoContext *)png_get_io_ptr(ps);

		if (ioContext->offset + size > ioContext->size) {
			png_error(ps, "Prematured end of image data");
			return;
		}

		memcpy(dest, ioContext->data + ioContext->offset, size);
	});

	png_read_png(ps, pi, PNG_TRANSFORM_IDENTITY, nullptr);

	widthOut = png_get_image_width(ps, pi);
	heightOut = png_get_image_height(ps, pi);

	size_t rowSize = png_get_rowbytes(ps, pi);
	texData = std::make_unique<char[]>(rowSize * heightOut);

	png_byte **rows = png_get_rows(ps, pi);

	for (size_t i = 0; i < heightOut; ++i) {
		memcpy(texData.get() + rowSize * i, rows[i], rowSize);
	}

	switch (png_get_color_type(ps, pi)) {
		case PNG_COLOR_TYPE_RGB:
			textureFormatOut = ghal::TextureFormat::RGB8;
		case PNG_COLOR_TYPE_RGBA:
			textureFormatOut = ghal::TextureFormat::RGBA8;
		default:
			textureFormatOut = ghal::TextureFormat::Unknown;
	}

	sizeOut = rowSize * heightOut;

	png_destroy_read_struct(&ps, &pi, nullptr);

	return texData.release();
}
