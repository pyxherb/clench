#ifndef _ENGINE_WINDOW_H_
#define _ENGINE_WINDOW_H_

#include <clench/wsal.h>
#include <clench/ghal.h>
#include <clench/vwc/root_window.h>
#include <memory>

namespace clench {
	namespace engine {
		class MainWindow : public vwc::VWCRootWindow {
		public:
			std::set<uint32_t> keyboardPressedKeys;

			CLCWSAL_API MainWindow(wsal::WindowScope *windowScope, wsal::NativeWindowHandle nativeWindowHandle);
			CLCWSAL_API virtual ~MainWindow();

			CLCWSAL_API virtual void onMove(int x, int y) override;
			CLCWSAL_API virtual bool onClose() override;

			CLCWSAL_API virtual bool isKeyDown(wsal::KeyboardKeyCode keyCode) const;

			CLCWSAL_API static MainWindow *alloc(wsal::WindowScope *windowScope, wsal::NativeWindowHandle nativeWindowHandle);
		};

		extern std::unique_ptr<wsal::WindowScope, peff::DeallocableDeleter> g_mainWindowScope;
		extern peff::RcObjectPtr<MainWindow> g_mainWindow;
	}
}

#endif
