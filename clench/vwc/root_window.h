#ifndef _CLENCH_VWC_ROOT_WINDOW_H_
#define _CLENCH_VWC_ROOT_WINDOW_H_

#include "control.h"

namespace clench {
	namespace vwc {
		class VWCRootWindow : public wsal::VirtualWindow {
		public:
			ghal::GHALDevice *ghalDevice;
			peff::RcObjectPtr<ghal::GHALDeviceContext> ghalDeviceContext;
			ghal::TextureFormat renderBufferFormat;
			wsal::LayoutAttributes layoutAttribs;

			CLCVWC_API VWCRootWindow(
				peff::Alloc *selfAllocator,
				wsal::Window *parent,
				ghal::GHALDevice *ghalDevice);
			CLCVWC_API virtual ~VWCRootWindow();

			CLCVWC_API virtual void dealloc() override;

			CLCVWC_API virtual void onResize(int width, int height) override;
			CLCVWC_API virtual void onMove(int x, int y) override;
			CLCVWC_API virtual bool onClose() override;
			CLCVWC_API virtual void onKeyDown(wsal::KeyboardKeyCode keyCode) override;
			CLCVWC_API virtual void onKeyUp(wsal::KeyboardKeyCode keyCode) override;
			CLCVWC_API virtual void onMouseButtonPress(wsal::MouseButton button, int x, int y) override;
			CLCVWC_API virtual void onMouseButtonRelease(wsal::MouseButton button, int x, int y) override;
			CLCVWC_API virtual void onMouseHover(int x, int y) override;
			CLCVWC_API virtual void onMouseLeave() override;
			CLCVWC_API virtual void onMouseMove(int x, int y) override;
			CLCVWC_API virtual void onDraw() override;

			CLCVWC_API void redrawChildWindows();
			CLCVWC_API void findWindowsAtPos(int x, int y, peff::Map<Window *, std::pair<int, int>> &childWindowsOut);

			CLCVWC_API const wsal::LayoutAttributes *getLayoutAttributes() const override;
		};
	}
}

#endif
