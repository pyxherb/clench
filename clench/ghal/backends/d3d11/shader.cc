#include "shader.h"

using namespace clench;
using namespace clench::ghal;

CLCGHAL_API D3D11VertexShader::D3D11VertexShader(
	Device *ownedDevice,
	ID3D11VertexShader *shader,
	const char *source,
	size_t szSource)
	: VertexShader(ownedDevice),
	  shader(shader),
	  source(std::make_unique<char[]>(szSource)),
	  szSource(szSource) {
	memcpy(this->source.get(), source, szSource);
}

CLCGHAL_API D3D11VertexShader::~D3D11VertexShader() {
}

CLCGHAL_API D3D11FragmentShader::D3D11FragmentShader(Device *ownedDevice, ID3D11PixelShader *shader) : FragmentShader(ownedDevice), shader(shader) {
}

CLCGHAL_API D3D11FragmentShader::~D3D11FragmentShader() {
}

CLCGHAL_API D3D11GeometryShader::D3D11GeometryShader(Device *ownedDevice, ID3D11GeometryShader *shader) : GeometryShader(ownedDevice), shader(shader) {
}

CLCGHAL_API D3D11GeometryShader::~D3D11GeometryShader() {
}

CLCGHAL_API D3D11ShaderProgram::D3D11ShaderProgram(
	Device *ownerDevice) : ShaderProgram(ownerDevice) {}
CLCGHAL_API D3D11ShaderProgram::~D3D11ShaderProgram() {}
