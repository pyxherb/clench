#ifndef _CLENCH_GRAPHICS_WIN32_WINDOW_H_
#define _CLENCH_GRAPHICS_WIN32_WINDOW_H_

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <clench/window/window.h>

namespace clench {
	namespace graphics {
		class Win32Window : public Window {
		protected:
			HWND _hWnd;

			static LRESULT _wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
			friend void clench::graphics::initWindow(CreateWindowFlags flags, std::wstring title);

		public:
			Win32Window(CreateWindowFlags flags, std::wstring title);
			virtual ~Win32Window();
			virtual void pollEvents() override;

			virtual void show() override;
			virtual void hide() override;

			virtual bool isVisible() override;

			virtual Eigen::Vector2i getSize() override;
			virtual void setSize(Eigen::Vector2i size) override;

			virtual void* getNativeHandle() override;
		};
	}
}

#endif
