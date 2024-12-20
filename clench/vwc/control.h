#ifndef _CLENCH_VWC_CONTROL_H_
#define _CLENCH_VWC_CONTROL_H_

#include "basedefs.h"

namespace clench {
	namespace vwc {
		class Control : public wsal::VirtualWindow {
		public:
			ghal::GHALDevice *ghalDevice;
			ghal::GHALDeviceContext *ghalDeviceContext;
			ghal::TextureFormat renderBufferFormat;

			std::unique_ptr<wsal::LayoutAttributes> layoutAttributes;

			CLCVWC_API Control(
				wsal::CreateWindowFlags createWindowFlags,
				ghal::GHALDevice *ghalDevice,
				ghal::GHALDeviceContext *ghalDeviceContext,
				ghal::TextureFormat renderBufferFormat,
				Window *parent,
				int x,
				int y,
				int width,
				int height);
			CLCVWC_API virtual ~Control();

			CLCVWC_API virtual void onResize(int width, int height) override;
			CLCVWC_API virtual void onExpose() override;

			CLCVWC_API virtual const wsal::LayoutAttributes *getLayoutAttributes() const;

			CLCVWC_API void redrawChildWindows();
			CLCVWC_API void repositionChildWindowsByLayout();
		};
	}
}

#endif
