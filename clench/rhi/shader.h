#ifndef _CLENCH_RHI_SHADER_H_
#define _CLENCH_RHI_SHADER_H_

#include "resource.h"

namespace clench {
	namespace rhi {
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

		class Shader : public RHIDeviceResource {
		public:
			const ShaderType shaderType;

			CLCRHI_API Shader(
				RHIDevice *ownedDevice,
				ShaderType shaderType);
			CLCRHI_API virtual ~Shader();
		};

		class VertexShader : public Shader {
		public:
			CLCRHI_API VertexShader(RHIDevice* ownedDevice);
			CLCRHI_API virtual ~VertexShader();
		};

		class FragmentShader : public Shader {
		public:
			CLCRHI_API FragmentShader(RHIDevice* ownedDevice);
			CLCRHI_API virtual ~FragmentShader();
		};

		class GeometryShader : public Shader {
		public:
			CLCRHI_API GeometryShader(RHIDevice* ownedDevice);
			CLCRHI_API virtual ~GeometryShader();
		};
	}
}

#endif
