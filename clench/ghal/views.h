#ifndef _CLENCH_GHAL_RENDER_TARGET_H_
#define _CLENCH_GHAL_RENDER_TARGET_H_

#include "resource.h"

namespace clench {
	namespace ghal {
		enum class RenderTargetViewType {
			Buffer = 0,
			Texture1D,
			Texture1DArray,
			Texture2D,
			Texture2DArray,
			Texture3D,
			Texture3DArray,
			Cube
		};

		class RenderTargetView : public DeviceResource {
		public:
			CLENCH_NO_COPY_MOVE_METHODS(RenderTargetView);

			RenderTargetViewType renderTargetViewType;

			CLCGHAL_API RenderTargetView(
				Device *ownerDevice,
				RenderTargetViewType renderTargetType);
			CLCGHAL_API virtual ~RenderTargetView();
		};

		enum class DepthStencilViewType {
			Texture1D = 0,
			Texture1DArray,
			Texture2D,
			Texture2DArray,
			Texture3D,
			Texture3DArray
		};

		class DepthStencilView : public DeviceResource {
		public:
			CLENCH_NO_COPY_MOVE_METHODS(DepthStencilView);

			DepthStencilViewType depthStencilViewType;

			CLCGHAL_API DepthStencilView(
				Device *ownerDevice,
				DepthStencilViewType depthStencilViewType);
			CLCGHAL_API virtual ~DepthStencilView();
		};
	}
}

#endif
