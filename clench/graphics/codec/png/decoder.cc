#include "decoder.h"

#include <png.h>

#include <stdexcept>

using namespace clench::graphics;

struct PNGIoContext {
	const char* data;
	std::size_t size;
	std::size_t offset;
};

PNGImageDecoder::PNGImageDecoder() : IImageDecoder("png") {}

void PNGImageDecoder::init() {}
void PNGImageDecoder::deinit() {}

RawTexture* PNGImageDecoder::decode(const char* data, size_t len) {
	png_structp ps;
	png_infop pi;
	PNGIoContext ioctxt;
	std::size_t width, height, szRow, szImage;
	TextureFormat pixelFmt;
	char* imgData = nullptr;

	// Create read & information structures
	if (!(ps = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr)))
		throw std::bad_alloc();
	if (!(pi = png_create_info_struct(ps))) {
		png_destroy_read_struct(&ps, nullptr, nullptr);
		throw std::bad_alloc();
	}
	// Setup error handler
	if (setjmp(png_jmpbuf(ps))) {
		if (imgData)
			delete[] imgData;
		png_destroy_read_struct(&ps, &pi, nullptr);
		throw ImageDecodeError("Error decoding image");
	}

	ioctxt.data = data;
	ioctxt.offset = 0;
	ioctxt.size = len;
	// Set PNG reader function
	png_set_read_fn(ps, &ioctxt, [](png_structp png, png_bytep data, png_size_t len) {
		PNGIoContext* ctxt = (PNGIoContext*)png_get_io_ptr(png);

		if (ctxt->offset + len > ctxt->size) {
			png_error(png, "Prematured image stream end");
			return;
		}

		std::memcpy(data, ((char*)(ctxt->data)) + ctxt->offset, len);
		ctxt->offset += len;
	});

	png_read_png(ps, pi, PNG_TRANSFORM_IDENTITY, nullptr);

	width = png_get_image_width(ps, pi);
	height = png_get_image_height(ps, pi);

	szRow = png_get_rowbytes(ps, pi);
	szImage = height * szRow;

	imgData = new char[szImage];

	auto rowptrs = png_get_rows(ps, pi);
	// Copy raw image data
	for (std::size_t i = 0; i < height; i++)
		std::memcpy(imgData + szRow * i, rowptrs[i], szRow);

	// Resolve image color type
	switch (png_get_color_type(ps, pi)) {
		case PNG_COLOR_TYPE_RGB:
			pixelFmt = TextureFormat::RGB8;
			break;
		case PNG_COLOR_TYPE_RGBA:
			pixelFmt = TextureFormat::RGBA8;
			break;
		case PNG_COLOR_TYPE_GRAY:
			pixelFmt = TextureFormat::GRAY8;
			break;
		case PNG_COLOR_TYPE_GRAY_ALPHA:
			pixelFmt = TextureFormat::GRAYALPHA8;
			break;
		default:
			pixelFmt = TextureFormat::UNKNOWN;
	}

	png_destroy_read_struct(&ps, &pi, nullptr);

	try {
		if ((width > INT_MAX) || (height > INT_MAX))
			throw std::runtime_error("Unacceptable image size");
		auto texture = new MemoryTexture(
			imgData,
			szImage,
			pixelFmt,
			Eigen::Vector2i((int)width, (int)height));
		delete[] imgData;
		return texture;
	} catch (...) {
		delete[] imgData;
		std::rethrow_exception(std::current_exception());
	}
}

PNGImageDecoder decoder;
