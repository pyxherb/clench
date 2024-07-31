#ifndef _CLENCH_WSI_WINDOW_H_
#define _CLENCH_WSI_WINDOW_H_

#include "basedefs.h"
#include "keyboard.h"
#include "mouse.h"
#include <string_view>
#include <set>
#include <map>

#ifdef _WIN32
	#include <Windows.h>
#elif defined(__unix__)
	#include <X11/Xlib.h>
	#include <X11/Xresource.h>
	#include <X11/Xutil.h>
	// #include <X11/extensions/Xrandr.h>
	#include <X11/Xatom.h>

	#undef None
#endif

namespace clench {
	namespace wsi {
		using CreateWindowFlags = std::uint32_t;
		constexpr CreateWindowFlags
			CREATEWINDOW_MIN = 1,
			CREATEWINDOW_MAX = 2,
			CREATEWINDOW_RESIZE = 4,
			CREATEWINDOW_NOFRAME = 8;

		constexpr int
			DEFAULT_WINDOW_POS = INT_MAX;

		class Window {
		private:
			bool _isClosed = false;

		public:
#ifdef _WIN32
			CLCWSI_API static LRESULT CALLBACK _win32WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif
#ifdef _WIN32
			HWND nativeHandle;
			std::set<uint32_t> keyboardPressedKeys;
#elif defined(__unix__)
			XID nativeHandle;
			XSetWindowAttributes _setWindowAttribs;
			XSizeHints *_sizeHints;
			std::map<uint32_t, Time> keyPressedTimes;
			std::set<uint32_t> keyboardPressedKeys;

			Display *display;
#endif

			NO_COPY_MOVE_METHODS(Window);

			CLCWSI_API Window(
				CreateWindowFlags flags,
				const char *title,
				Window *parent,
				int x,
				int y,
				int width,
				int height);
			CLCWSI_API virtual ~Window();

			virtual void onResize(int width, int height) = 0;
			virtual void onMove(int x, int y) = 0;
			virtual bool onClose() = 0;
			virtual void onMouseButtonPress(MouseButton button, int x, int y) = 0;
			virtual void onMouseButtonRelease(MouseButton button, int x, int y) = 0;
			virtual void onMouseHover(int x, int y) = 0;
			virtual void onMouseLeave(int x, int y) = 0;
			virtual void onMouseMove(int x, int y) = 0;

			CLCWSI_API void show();
			CLCWSI_API void hide();
			CLCWSI_API bool isVisible() const;

			CLCWSI_API bool isClosed() const;

			CLCWSI_API void setPos(int x, int y);
			CLCWSI_API void getPos(int &xOut, int &yOut) const;

			CLCWSI_API void setSize(int width, int height);
			CLCWSI_API void getSize(int &widthOut, int &heightOut) const;

			CLCWSI_API bool isKeyDown(KeyboardKeyCode keyCode) const;

			CLCWSI_API void setTitle(const char *title);

			CLCWSI_API void pollEvents();
		};
	}
}

#endif
