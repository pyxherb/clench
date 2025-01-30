#include "texture.h"

using namespace clench;
using namespace clench::ghal;

CLCGHAL_API D3D11Texture1D::D3D11Texture1D(
	Device *ownerDevice,
	const Texture1DDesc &textureDesc,
	ID3D11Texture1D *texture1D)
	: Texture1D(ownerDevice, textureDesc),
	  texture1D(texture1D) {
}

CLCGHAL_API D3D11Texture1D::~D3D11Texture1D() {
}

CLCGHAL_API D3D11Texture2D::D3D11Texture2D(
	Device *ownerDevice,
	const Texture2DDesc &textureDesc,
	ID3D11Texture2D *texture1D)
	: Texture2D(ownerDevice, textureDesc),
	  texture2D(texture2D) {
}

CLCGHAL_API D3D11Texture2D::~D3D11Texture2D() {
}

CLCGHAL_API D3D11Texture3D::D3D11Texture3D(
	Device *ownerDevice,
	const Texture3DDesc &textureDesc,
	ID3D11Texture3D *texture1D)
	: Texture3D(ownerDevice, textureDesc),
	  texture3D(texture3D) {
}

CLCGHAL_API D3D11Texture3D::~D3D11Texture3D() {
}
