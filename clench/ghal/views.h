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

		class RenderTargetView : public GHALDeviceResource {
		public:
			NO_COPY_MOVE_METHODS(RenderTargetView);

			RenderTargetViewType renderTargetViewType;

			CLCGHAL_API RenderTargetView(
				GHALDevice *ownerDevice,
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

		class DepthStencilView : public GHALDeviceResource {
		public:
			NO_COPY_MOVE_METHODS(DepthStencilView);

			DepthStencilViewType depthStencilViewType;

			CLCGHAL_API DepthStencilView(
				GHALDevice *ownerDevice,
				DepthStencilViewType depthStencilViewType);
			CLCGHAL_API virtual ~DepthStencilView();
		};
	}
}

#endif
