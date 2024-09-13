#ifndef _ENGINE_WINDOW_H_
#define _ENGINE_WINDOW_H_

#include <clench/wsal.h>
#include <clench/ghal.h>
#include <memory>

namespace clench {
	namespace engine {
		class MainWindow :public wsal::NativeWindow {
		public:
			std::set<uint32_t> keyboardPressedKeys;

			CLCWSAL_API MainWindow();
			CLCWSAL_API virtual ~MainWindow();

			CLCWSAL_API virtual void onResize(int width, int height) override;
			CLCWSAL_API virtual void onMove(int x, int y) override;
			CLCWSAL_API virtual bool onClose() override;
			CLCWSAL_API virtual void onMouseButtonPress(wsal::MouseButton button, int x, int y) override;
			CLCWSAL_API virtual void onMouseButtonRelease(wsal::MouseButton button, int x, int y) override;
			CLCWSAL_API virtual void onMouseHover(int x, int y) override;
			CLCWSAL_API virtual void onMouseLeave() override;
			CLCWSAL_API virtual void onMouseMove(int x, int y) override;

			CLCWSAL_API virtual bool isKeyDown(wsal::KeyboardKeyCode keyCode) const;
		};

		extern std::unique_ptr<MainWindow> g_mainWindow;
	}
}

#endif
