#ifndef _CLENCH_GRAPHICS_X11_WINDOW_H_
#define _CLENCH_GRAPHICS_X11_WINDOW_H_

#include <X11/Xlib.h>
#include <X11/Xresource.h>
#include <X11/Xutil.h>
#include <X11/extensions/Xrandr.h>
#include <X11/Xatom.h>

// X11 has already defined `Success' ahead of Eigen, we have to undefine it.
#undef Success

#include <clench/window/window.h>

#include <codecvt>
#include <locale>
#include <set>

namespace clench {
	namespace graphics {
		class X11Window : public Window {
		public:
			XID _window;
			XSetWindowAttributes _setWindowAttribs;
			std::wstring_convert<std::codecvt_utf8<wchar_t>> _codecvt;
			XSizeHints* _sizeHints;
			std::set<uint32_t> _keyboardPressedKeys;

			bool _isVisible = false;
			bool _isClosed = false;

			Display* display;

			X11Window(CreateWindowFlags flags, std::wstring title);
			virtual ~X11Window();
			virtual void pollEvents() override;

			virtual void show() override;
			virtual void hide() override;

			virtual bool isVisible() const override;

			virtual inline bool isClosed() const { return _isClosed; }

			virtual Eigen::Vector2i getSize() const override;
			virtual void setSize(Eigen::Vector2i size) override;

			virtual unsigned int getSystemDpi() const override;

			virtual void* getNativeHandle() const override;

			virtual WindowBackend& getBackend() const override;

			virtual bool isKeyDown(input::KeyboardKeyCode keyCode) const override;
		};

		class X11WindowBackend : public WindowBackend {
		public:
			X11WindowBackend();
			virtual ~X11WindowBackend() = default;

			virtual Window* createWindow(CreateWindowFlags flags, std::wstring title) override;
		};
	}
}

#endif
