#ifndef _ENGINE_WINDOW_H_
#define _ENGINE_WINDOW_H_

#include <clench/wsi.h>
#include <clench/rhi.h>
#include <memory>

namespace clench {
	namespace engine {
		class MainWindow :public wsi::Window {
		public:
			MainWindow();
			virtual ~MainWindow();

			virtual void onResize(int width, int height) override;
			virtual void onMove(int x, int y) override;
			virtual bool onClose() override;
			virtual void onMouseButtonPress(wsi::MouseButton button, int x, int y) override;
			virtual void onMouseButtonRelease(wsi::MouseButton button, int x, int y) override;
			virtual void onMouseHover(int x, int y) override;
			virtual void onMouseLeave(int x, int y) override;
			virtual void onMouseMove(int x, int y) override;
		};

		extern std::unique_ptr<MainWindow> g_mainWindow;
	}
}

#endif
