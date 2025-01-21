#ifndef _CLENCH_WSAL_BACKENDS_X11_WINDOW_H_
#define _CLENCH_WSAL_BACKENDS_X11_WINDOW_H_

#include <clench/wsal/window.h>
#include <X11/Xlib.h>
#include <X11/Xresource.h>
#include <X11/Xutil.h>
// #include <X11/extensions/Xrandr.h>
#include <X11/Xatom.h>
#include <X11/Xproto.h>
#include <X11/extensions/shapeproto.h>
#include <peff/containers/set.h>
#include <peff/containers/map.h>

constexpr static auto X11_None = 0L;
#undef None

namespace clench {
	namespace wsal {
		class X11Backend;

		struct X11WindowHandle {
			::Window window;
			::Display *display;

			CLENCH_FORCEINLINE bool operator<(const X11WindowHandle &rhs) const {
				if (display < rhs.display)
					return true;
				if (display > rhs.display)
					return false;
				if (window < rhs.window)
					return true;
				return window > rhs.window;
			}
		};

		class X11Window : public Window {
		private:
			bool _isClosed = false;

		public:
			bool wasCursorInCapturedWindow = false;
			bool wasCursorInCapturedRootWindow = false;
			Window *curCapturedWindow = nullptr;

			X11WindowHandle nativeHandle;

			XSetWindowAttributes _setWindowAttribs;
			XSizeHints *_sizeHints;
			peff::Map<uint32_t, Time> _keyPressedTimes;

			peff::Set<peff::RcObjectPtr<VirtualWindow>> _childVirtualWindows;
			peff::Set<Window *> hoveredChildWindows;

			CLENCH_NO_COPY_MOVE_METHODS(X11Window);

			CLCWSAL_API X11Window(
				X11Backend *backend,
				X11WindowHandle nativeHandle);
			CLCWSAL_API virtual ~X11Window();

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

			CLCWSAL_API virtual void addChildWindow(Window *window) override;
			CLCWSAL_API virtual void removeChildWindow(Window *window) override;
			CLCWSAL_API virtual bool hasChildWindow(Window *window) const override;

			CLCWSAL_API virtual void enumChildWindows(ChildWindowEnumer &&enumer) override;

			CLCWSAL_API virtual void getWindowProperties(WindowProperties &propertiesOut) const override;

			CLCWSAL_API virtual void invalidate() override;

			CLCWSAL_API virtual void pollEvents() override;

			CLCWSAL_API virtual void onResize(int width, int height) override;
			CLCWSAL_API virtual void onMove(int x, int y) override;
			CLCWSAL_API virtual bool onClose() override;
			CLCWSAL_API virtual void onKeyDown(KeyboardKeyCode keyCode) override;
			CLCWSAL_API virtual void onKeyUp(KeyboardKeyCode keyCode) override;
			CLCWSAL_API virtual void onMouseButtonPress(MouseButton button, int x, int y) override;
			CLCWSAL_API virtual void onMouseButtonRelease(MouseButton button, int x, int y) override;
			CLCWSAL_API virtual void onMouseHover(int x, int y) override;
			CLCWSAL_API virtual void onMouseLeave() override;
			CLCWSAL_API virtual void onMouseMove(int x, int y) override;
			CLCWSAL_API virtual void onExpose() override;
			CLCWSAL_API virtual void onDraw() override;

			CLCWSAL_API void findWindowsAtPos(int x, int y, peff::Map<clench::wsal::VirtualWindow *, std::pair<int, int>> &childWindowsOut);

			CLCWSAL_API static X11Window *alloc(
				X11Backend *backend,
				X11WindowHandle nativeHandle);
		};
	}
}

#endif
