#include "shader.h"

using namespace clench;
using namespace clench::rhi;

CLCRHI_API D3D11VertexShader::D3D11VertexShader(RHIDevice *ownedDevice, ID3D11VertexShader *shader) : VertexShader(ownedDevice), shader(shader) {

}

CLCRHI_API D3D11VertexShader::~D3D11VertexShader() {

}

CLCRHI_API D3D11FragmentShader::D3D11FragmentShader(RHIDevice *ownedDevice, ID3D11PixelShader *shader) : FragmentShader(ownedDevice), shader(shader) {

}

CLCRHI_API D3D11FragmentShader::~D3D11FragmentShader() {

}

CLCRHI_API D3D11GeometryShader::D3D11GeometryShader(RHIDevice *ownedDevice, ID3D11GeometryShader *shader) : GeometryShader(ownedDevice), shader(shader) {

}

CLCRHI_API D3D11GeometryShader::~D3D11GeometryShader() {

}
