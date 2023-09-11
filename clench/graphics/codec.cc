#include "codec.h"
#include <clench/debug.h>

#include <map>
#include <stdexcept>
#include <memory>

using namespace clench::graphics;

std::unique_ptr<std::map<std::string, IImageDecoder*>> _registeredDecoders;

IImageDecoder::IImageDecoder(std::string name) {
	if (!_registeredDecoders)
		_registeredDecoders = std::make_unique<std::map<std::string, IImageDecoder*>>();
	(*_registeredDecoders)[name] = this;
	clench::debug::info("Registered image decoder: " + name);
}

void clench::graphics::initImageDecoders() {
	for (auto& i : *_registeredDecoders)
		i.second->init();
}

void clench::graphics::deinitImageDecoders() {
	for (auto& i : *_registeredDecoders)
		i.second->deinit();
}

///
/// @brief Try to decode an raw image with all registered decoders.
///
/// @param data Pointer of the raw image.
/// @param len Size of the raw image.
/// @return Decoded image.
///
RawTexture* clench::graphics::decodeImage(const char* data, size_t len) {
	RawTexture* texture;

	for (auto i : *_registeredDecoders) {
		try {
			texture = i.second->decode(data, len);
		} catch (ImageDecodeError e) {
			continue;
		}
		clench::debug::info("Decoded raw data %p with decoder: %s", data, i.first.c_str());
		return texture;
	}

	throw std::runtime_error("No compatible image decoder found");
}
