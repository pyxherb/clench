#ifndef _CLENCH_VWC_ROOT_WINDOW_H_
#define _CLENCH_VWC_ROOT_WINDOW_H_

#include "control.h"

namespace clench {
	namespace vwc {
		class VWCRootWindow : public wsal::NativeWindow {
		public:
			std::set<peff::RcObjectPtr<Window>> childWindows;
			ghal::GHALDevice *ghalDevice;
			std::unique_ptr<ghal::GHALDeviceContext> ghalDeviceContext;
			ghal::TextureFormat renderBufferFormat;
			std::set<Window *> hoveredChildWindows;

			CLCVWC_API VWCRootWindow(
				wsal::WindowScope *windowScope,
				wsal::NativeWindowHandle nativeWindowHandle,
				ghal::GHALDevice *ghalDevice);
			CLCVWC_API virtual ~VWCRootWindow();

			CLCVWC_API virtual void addChildWindow(Window *window) override;
			CLCVWC_API virtual void removeChildWindow(Window *window) override;
			CLCVWC_API virtual bool hasChildWindow(Window *window) const override;

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
			CLCVWC_API void findWindowsAtPos(int x, int y, std::map<Window *, std::pair<int, int>> &childWindowsOut);
		};
	}
}

#endif
