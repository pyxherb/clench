#ifndef _CLENCH_WSAL_WIN32_WINDOW_H_
#define _CLENCH_WSAL_WIN32_WINDOW_H_

#include <clench/wsal/window.h>
#include <Windows.h>
#include <windowsx.h>

#define CLENCH_WNDCLASS_NAME "CLENCH"

namespace clench {
	namespace wsal {
		class Win32Backend;

		using Win32WindowHandle = HWND;

		class Win32Window : public NativeWindow {
		private:
			bool _isClosed = false;

		public:
			bool isHovered = false;
			Window *curCapturedWindow = nullptr;

			Win32WindowHandle nativeHandle;

			peff::Set<Window *> hoveredChildWindows;

			CLENCH_NO_COPY_MOVE_METHODS(Win32Window);

			CLCWSAL_API static LRESULT CALLBACK _win32WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

			CLCWSAL_API Win32Window(
				Win32Backend *backend,
				Win32WindowHandle nativeHandle);
			CLCWSAL_API virtual ~Win32Window();

			CLCWSAL_API virtual void dealloc() override;

			CLCWSAL_API virtual void show() override;
			CLCWSAL_API virtual void hide() override;
			CLCWSAL_API virtual bool isVisible() const override;

			CLCWSAL_API virtual bool isClosed() const override;

			CLCWSAL_API virtual void setPos(int x, int y) override;
			CLCWSAL_API virtual void getPos(int &xOut, int &yOut) const override;

			CLCWSAL_API virtual void setSize(int width, int height) override;
			CLCWSAL_API virtual void getSize(int &widthOut, int &heightOut) const override;

			CLCWSAL_API virtual void setTitle(const char *title) override;

			CLCWSAL_API virtual void setParent(Window *window) override;
			CLCWSAL_API virtual Window *getParent() const override;

			CLCWSAL_API virtual base::ExceptionPtr addChildWindow(Window *window) override;
			CLCWSAL_API virtual void removeChildWindow(Window *window) override;
			CLCWSAL_API virtual bool hasChildWindow(Window *window) const override;

			CLCWSAL_API virtual void enumChildWindows(ChildWindowEnumer &&enumer) override;

			CLCWSAL_API virtual void getWindowProperties(WindowProperties &propertiesOut) const override;

			CLCWSAL_API virtual void invalidate() override;

			CLCWSAL_API virtual void pollEvents() override;

			CLCWSAL_API static Win32Window *alloc(
				Win32Backend *backend,
				Win32WindowHandle nativeHandle);
		};
	}
}

#endif
