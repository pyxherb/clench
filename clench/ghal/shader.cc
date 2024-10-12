#include "shader.h"

using namespace clench;
using namespace clench::ghal;

CLCGHAL_API Shader::Shader(
	GHALDevice* ownedDevice,
	ShaderType shaderType)
	:GHALDeviceResource(ownedDevice),
	shaderType(shaderType)
{
}

CLCGHAL_API Shader::~Shader() {
}

CLCGHAL_API VertexShader::VertexShader(GHALDevice* ownedDevice)
	: Shader(ownedDevice, ShaderType::Vertex) {
}

CLCGHAL_API VertexShader::~VertexShader() {
}

CLCGHAL_API FragmentShader::FragmentShader(GHALDevice* ownedDevice)
	: Shader(ownedDevice, ShaderType::Fragment) {
}

CLCGHAL_API FragmentShader::~FragmentShader() {
}

CLCGHAL_API GeometryShader::GeometryShader(GHALDevice* ownedDevice)
	: Shader(ownedDevice, ShaderType::Vertex) {
}

CLCGHAL_API GeometryShader::~GeometryShader() {
}

CLCGHAL_API ShaderProgram::ShaderProgram(GHALDevice *ownerDevice) : GHALDeviceResource(ownerDevice) {}

CLCGHAL_API ShaderProgram::~ShaderProgram() {
}
