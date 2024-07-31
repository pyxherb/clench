#include "shader.h"

using namespace clench;
using namespace clench::rhi;

CLCRHI_API Shader::Shader(
	RHIDevice* ownedDevice,
	ShaderType shaderType)
	:RHIDeviceResource(ownedDevice),
	shaderType(shaderType)
{
}

CLCRHI_API Shader::~Shader() {
}

CLCRHI_API VertexShader::VertexShader(RHIDevice* ownedDevice)
	: Shader(ownedDevice, ShaderType::Vertex) {
}

CLCRHI_API VertexShader::~VertexShader() {
}

CLCRHI_API FragmentShader::FragmentShader(RHIDevice* ownedDevice)
	: Shader(ownedDevice, ShaderType::Fragment) {
}

CLCRHI_API FragmentShader::~FragmentShader() {
}

CLCRHI_API GeometryShader::GeometryShader(RHIDevice* ownedDevice)
	: Shader(ownedDevice, ShaderType::Vertex) {
}

CLCRHI_API GeometryShader::~GeometryShader() {
}