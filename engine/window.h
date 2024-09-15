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

			MainWindow();
			virtual ~MainWindow();

			virtual void onResize(int width, int height) override;
			virtual void onMove(int x, int y) override;
			virtual bool onClose() override;
			virtual void onMouseButtonPress(wsal::MouseButton button, int x, int y) override;
			virtual void onMouseButtonRelease(wsal::MouseButton button, int x, int y) override;
			virtual void onMouseHover(int x, int y) override;
			virtual void onMouseLeave() override;
			virtual void onMouseMove(int x, int y) override;

			virtual bool isKeyDown(wsal::KeyboardKeyCode keyCode) const;
		};

		extern std::unique_ptr<MainWindow> g_mainWindow;
	}
}

#endif
