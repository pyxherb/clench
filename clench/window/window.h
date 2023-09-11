#ifndef _CLENCH_GRAPHICS_WINDOW_H_
#define _CLENCH_GRAPHICS_WINDOW_H_

#include <clench/math.h>
#include <clench/input.h>

#include <deque>
#include <functional>
#include <memory>

namespace clench {
	namespace graphics {
		using CreateWindowFlags = std::uint32_t;
		constexpr CreateWindowFlags CREATEWINDOW_MIN = 1,
									CREATEWINDOW_MAX = 2,
									CREATEWINDOW_RESIZE = 4;

		constexpr int DEFAULT_WINDOW_WIDTH = 1280, DEFAULT_WINDOW_HEIGHT = 960, DEFAULT_DPI = 72;

		class WindowBackend;

		class Window {
		public:
			std::function<void(Eigen::Vector2i size)> onResize = [](Eigen::Vector2i) {};
			std::function<void(Eigen::Vector2i pos)> onMove = [](Eigen::Vector2i) {};
			std::function<bool()> onClose = [] { return true; };

			virtual inline ~Window(){};

			virtual void pollEvents() = 0;

			virtual void show() = 0;
			virtual void hide() = 0;
			virtual bool isVisible() const = 0;

			virtual inline bool isClosed() const = 0;

			virtual Eigen::Vector2i getSize() const = 0;
			virtual void setSize(Eigen::Vector2i size) = 0;

			virtual unsigned int getSystemDpi() const = 0;

			virtual void* getNativeHandle() const = 0;

			virtual WindowBackend& getBackend() const = 0;

			virtual bool isKeyDown(input::KeyboardKeyCode) const = 0;
		};

		class WindowBackend {
		private:
			std::string _name;

		public:
			WindowBackend(std::string name);
			virtual ~WindowBackend() = default;

			virtual Window* createWindow(CreateWindowFlags flags, std::wstring title) = 0;

			inline std::string getName() { return _name; }
		};

		Window* createWindow(
			CreateWindowFlags flags,
			std::wstring title,
			std::deque<std::string> preferredBackends = {});
	}
}

#endif
