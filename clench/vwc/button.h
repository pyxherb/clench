#ifndef _CLENCH_VWC_BUTTON_H_
#define _CLENCH_VWC_BUTTON_H_

#include "control.h"

namespace clench {
	namespace vwc {
		class Button : public Control {
		public:
			CLCVWC_API Button(
				ghal::GHALDevice *ghalDevice,
				ghal::GHALDeviceContext *ghalDeviceContext,
				ghal::TextureFormat renderBufferFormat,
				Window *parent,
				int x,
				int y,
				int width,
				int height);
			CLCVWC_API virtual ~Button();

			CLCVWC_API virtual void onMouseButtonPress(wsal::MouseButton button, int x, int y) override;
			CLCVWC_API virtual void onMouseButtonRelease(wsal::MouseButton button, int x, int y) override;
			CLCVWC_API virtual void onMouseHover(int x, int y) override;
			CLCVWC_API virtual void onMouseLeave() override;

			virtual void onHover() = 0;
			virtual void onLeave() = 0;
			virtual void onPress() = 0;
			virtual void onRelease() = 0;
			virtual bool isPressed() const = 0;
		};

		class DefaultButton : public Button {
		private:
			bool _isHovered = false;
			bool _isPressed = false;

		public:
			utils::RcObjectPtr<Control> content;

			float backgroundColor[3] = { 0.7f, 0.7f, 0.7f };
			float pressedBackgroundColor[3] = { 0.6f, 0.6f, 0.6f };
			float hoverBackgroundColor[3] = { 0.8f, 0.8f, 0.8f };

			CLCVWC_API DefaultButton(
				ghal::GHALDevice *ghalDevice,
				ghal::GHALDeviceContext *ghalDeviceContext,
				ghal::TextureFormat renderBufferFormat,
				Window *parent,
				int x,
				int y,
				int width,
				int height);
			CLCVWC_API virtual ~DefaultButton();

			CLCVWC_API virtual void onDraw() override;

			CLCVWC_API virtual void onHover() override;
			CLCVWC_API virtual void onLeave() override;
			CLCVWC_API virtual void onPress() override;
			CLCVWC_API virtual void onRelease() override;
			CLCVWC_API virtual bool isPressed() const override;
		};
	}
}

#endif
