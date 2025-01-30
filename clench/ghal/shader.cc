#include "shader.h"

using namespace clench;
using namespace clench::ghal;

CLCGHAL_API Shader::Shader(
	Device* ownedDevice,
	ShaderType shaderType)
	:DeviceResource(ownedDevice),
	shaderType(shaderType)
{
}

CLCGHAL_API Shader::~Shader() {
}

CLCGHAL_API VertexShader::VertexShader(Device* ownedDevice)
	: Shader(ownedDevice, ShaderType::Vertex) {
}

CLCGHAL_API VertexShader::~VertexShader() {
}

CLCGHAL_API FragmentShader::FragmentShader(Device* ownedDevice)
	: Shader(ownedDevice, ShaderType::Fragment) {
}

CLCGHAL_API FragmentShader::~FragmentShader() {
}

CLCGHAL_API GeometryShader::GeometryShader(Device* ownedDevice)
	: Shader(ownedDevice, ShaderType::Vertex) {
}

CLCGHAL_API GeometryShader::~GeometryShader() {
}

CLCGHAL_API ShaderProgram::ShaderProgram(Device *ownerDevice) : DeviceResource(ownerDevice) {}

CLCGHAL_API ShaderProgram::~ShaderProgram() {
}
