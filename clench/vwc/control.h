#ifndef _CLENCH_VWC_CONTROL_H_
#define _CLENCH_VWC_CONTROL_H_

#include "basedefs.h"
#include <clench/acri/device.h>

namespace clench {
	namespace vwc {
		class Control : public wsal::VirtualWindow {
		public:
			acri::Device *acriDevice;
			acri::DeviceContext *acriDeviceContext;
			ghal::TextureFormat renderBufferFormat;

			std::unique_ptr<wsal::LayoutAttributes> layoutAttributes;

			CLCVWC_API Control(
				peff::Alloc *selfAllocator,
				wsal::CreateWindowFlags createWindowFlags,
				acri::Device *acriDevice,
				acri::DeviceContext *acriDeviceContext,
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
