#ifndef _CLENCH_GHAL_SHADER_H_
#define _CLENCH_GHAL_SHADER_H_

#include "resource.h"
#include <string>

namespace clench {
	namespace ghal {
		enum class InputVertexShaderSemanticType : uint8_t {
			None = 0,
			Binormal,
			BlendIndices,
			BlendWeight,
			Color,
			Normal,
			Position,
			TransformPosition,
			PointSize,
			Tangent,
			TextureCoord
		};

		enum class OutputVertexShaderSemanticType : uint8_t {
			None = 0,
			Color,
			Fog,
			Position,
			PointSize,
			TessFactor
		};

		enum class InputFragmentShaderSemanticType : uint8_t {
			None = 0,
			Color,
			TextureCoord,
			VFace,
			VPos
		};

		enum class OutputFragmentShaderSemanticType : uint8_t {
			None = 0,
			Color,
			Depth
		};

		enum class ShaderType {
			Vertex = 0,
			Fragment,
			Geometry
		};

		struct ShaderSourceInfo {
			std::string sourcePath;
		};

		class Shader : public GHALDeviceResource {
		public:
			const ShaderType shaderType;

			CLCGHAL_API Shader(
				GHALDevice *ownedDevice,
				ShaderType shaderType);
			CLCGHAL_API virtual ~Shader();
		};

		class VertexShader : public Shader {
		public:
			CLCGHAL_API VertexShader(GHALDevice* ownedDevice);
			CLCGHAL_API virtual ~VertexShader();
		};

		class FragmentShader : public Shader {
		public:
			CLCGHAL_API FragmentShader(GHALDevice* ownedDevice);
			CLCGHAL_API virtual ~FragmentShader();
		};

		class GeometryShader : public Shader {
		public:
			CLCGHAL_API GeometryShader(GHALDevice* ownedDevice);
			CLCGHAL_API virtual ~GeometryShader();
		};

		class ShaderProgram : public GHALDeviceResource {
		public:
			CLCGHAL_API ShaderProgram(
				GHALDevice *ownerDevice
			);
			CLCGHAL_API virtual ~ShaderProgram();
		};
	}
}

#endif
