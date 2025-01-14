#ifndef _CLENCH_WSAL_WINDOW_H_
#define _CLENCH_WSAL_WINDOW_H_

#include "basedefs.h"
#include "keyboard.h"
#include "mouse.h"
#include <peff/base/rcobj.h>
#include <peff/base/deallocable.h>
#include <peff/base/allocator.h>
#include <peff/containers/set.h>
#include <peff/containers/map.h>
#include <clench/utils/bitarray.h>
#include <string_view>
#include <set>
#include <map>
#include <functional>
#include <climits>
#include <string>
#include <stdexcept>

#ifdef _WIN32
	#include <Windows.h>
#elif defined(__unix__)
	#include <X11/Xlib.h>
	#include <X11/Xresource.h>
	#include <X11/Xutil.h>
	// #include <X11/extensions/Xrandr.h>
	#include <X11/Xatom.h>
	#include <X11/Xproto.h>
	#include <X11/extensions/shapeproto.h>

constexpr static auto X11_None = 0L;
	#undef None
#endif

namespace clench {
	namespace wsal {
		class Window;

#ifdef _WIN32
		using NativeWindowHandle = HWND;
#elif defined(__unix__)
		struct NativeWindowHandle {
			enum class Kind {
				X11 = 0,
				Wayland
			};
			union {
				struct {
					XID windowId;
					Display *display;
				} x11;
			} data;
			Kind kind;

			NativeWindowHandle() = default;
			CLENCH_FORCEINLINE NativeWindowHandle(Display *display, XID windowId) : kind(Kind::X11) {
				data.x11.display = display;
				data.x11.windowId = windowId;
			}

			CLENCH_FORCEINLINE bool operator<(const NativeWindowHandle &rhs) const {
				if (kind < rhs.kind) {
					return true;
				} else if (kind > rhs.kind) {
					return false;
				}

				switch (kind) {
					case Kind::X11:
						if (data.x11.display > rhs.data.x11.display) {
							return false;
						} else if (data.x11.display < rhs.data.x11.display) {
							return true;
						}
						if (data.x11.windowId > rhs.data.x11.windowId) {
							return false;
						}
						if (data.x11.windowId < rhs.data.x11.windowId) {
							return true;
						}
						break;
					default:
						assert(false);
				}
				return false;
			}
		};
#endif

		class WindowScope final : public peff::Deallocable {
		private:
			CLCWSAL_API WindowScope(peff::Alloc *selfAllocator, peff::Alloc *allocator);

		public:
			peff::RcObjectPtr<peff::Alloc> selfAllocator, allocator;
			peff::Set<Window *> childWindows;
			peff::Map<NativeWindowHandle, Window *> handleToWindowMap;

			CLCWSAL_API ~WindowScope();

			CLCWSAL_API virtual void dealloc() override;

			CLCWSAL_API static WindowScope *alloc(peff::Alloc *selfAllocator, peff::Alloc *allocator);
		};

		using CreateWindowFlags = std::uint32_t;
		constexpr CreateWindowFlags
			CREATEWINDOW_MIN = 1,
			CREATEWINDOW_MAX = 2,
			CREATEWINDOW_RESIZE = 4,
			CREATEWINDOW_NOFRAME = 8;

		constexpr int
			DEFAULT_WINDOW_POS = INT_MAX;

		class Window;

		class WindowClipping {
		public:
			size_t width, height;
			utils::BitArray bitArray;

			void resize(size_t width, size_t height) {
				this->width = width;
				this->height = height;

				bitArray.resize(width * height);
			}

			bool getByPos(size_t x, size_t y) {
				return bitArray.getBit(width * y + x);
			}

			void setByPos(size_t x, size_t y) {
				bitArray.setBit(width * y + x);
			}

			void clearByPos(size_t x, size_t y) {
				bitArray.clearBit(width * y + x);
			}
		};

		using ChildWindowEnumer = std::function<
			bool(Window *window)>;

		struct WindowProperties {
			bool isNative : 1;
		};

		class Window : public peff::RcObject {
		public:
			WindowScope *windowScope;

			CLENCH_NO_COPY_MOVE_METHODS(Window);

			CLCWSAL_API Window(WindowScope *windowScope);
			CLCWSAL_API virtual ~Window();

			virtual void onRefZero() noexcept override;

			virtual void show() = 0;
			virtual void hide() = 0;
			virtual bool isVisible() const = 0;

			virtual bool isClosed() const = 0;

			virtual void setPos(int x, int y) = 0;
			virtual void getPos(int &xOut, int &yOut) const = 0;

			virtual void setSize(int width, int height) = 0;
			virtual void getSize(int &widthOut, int &heightOut) const = 0;

			// virtual void getActualPos(int &xOut, int &yOut) const = 0;
			// virtual void getActualSize(int &widthOut, int &heightOut) const = 0;

			virtual void setTitle(const char *title) = 0;

			virtual void setParent(Window *window) = 0;
			virtual Window *getParent() const = 0;

			virtual void addChildWindow(Window *window) = 0;
			virtual void removeChildWindow(Window *window) = 0;
			virtual bool hasChildWindow(Window *window) const = 0;

			virtual void enumChildWindows(ChildWindowEnumer &&enumer) = 0;

			virtual void getWindowProperties(WindowProperties &propertiesOut) const = 0;

			virtual void invalidate() = 0;

			virtual void pollEvents() = 0;

			virtual void onResize(int width, int height) = 0;
			virtual void onMove(int x, int y) = 0;
			virtual bool onClose() = 0;
			virtual void onKeyDown(KeyboardKeyCode keyCode) = 0;
			virtual void onKeyUp(KeyboardKeyCode keyCode) = 0;
			virtual void onMouseButtonPress(MouseButton button, int x, int y) = 0;
			virtual void onMouseButtonRelease(MouseButton button, int x, int y) = 0;
			virtual void onMouseHover(int x, int y) = 0;
			virtual void onMouseLeave() = 0;
			virtual void onMouseMove(int x, int y) = 0;
			virtual void onExpose() = 0;
			virtual void onDraw() = 0;
		};

		class NativeWindow : public Window {
		private:
			bool _isClosed = false;

		protected:
			std::set<Window *> prevHoveredWindows;

		public:
			bool wasCursorInCapturedWindow = false;
			bool wasCursorInCapturedRootWindow = false;
			Window *curCapturedWindow = nullptr;

#ifdef _WIN32
			CLCWSAL_API static LRESULT CALLBACK _win32WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif

			NativeWindowHandle nativeHandle;
#ifdef _WIN32
			bool isHovered = false;
#elif defined(__unix__)
			XSetWindowAttributes _setWindowAttribs;
			XSizeHints *_sizeHints;
			std::map<uint32_t, Time> _keyPressedTimes;
#endif

			CLENCH_NO_COPY_MOVE_METHODS(NativeWindow);

			CLCWSAL_API NativeWindow(
				WindowScope *windowScope,
				CreateWindowFlags flags,
				NativeWindow *parent,
				int x,
				int y,
				int width,
				int height);
			CLCWSAL_API virtual ~NativeWindow();

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
		};

		constexpr static int UNSET = INT_MIN;

		struct Box {
			int top = UNSET, bottom = UNSET, left = UNSET, right = UNSET;

			Box() = default;
			Box(const Box &) = default;
			Box(Box &&) = default;
			CLENCH_FORCEINLINE Box(int top, int bottom, int left, int right) : top(top), bottom(bottom), left(left), right(right) {}
			CLENCH_FORCEINLINE Box(int value) : top(value), bottom(value), left(value), right(value) {}
			~Box() = default;

			Box &operator=(const Box &) = default;
			Box &operator=(Box &&) = default;
		};

		enum class HorizontalAlignment {
			Unset = 0,
			Left,
			Center,
			Right,
			Stretch
		};

		enum class VerticalAlignment {
			Unset = 0,
			Top,
			Center,
			Bottom,
			Stretch
		};

		enum class LayoutValueType {
			Unset = 0,	  // Unset
			Integer,	  // Integer
			Float,		  // Floating-point number
			SizeScale,	  // Size scale to the parent window
			WidthScale,	  // Width scale to the parent window
			HeightScale,  // Height scale to the parent window
		};

		struct LayoutValue {
			union {
				int asInteger;
				float asFloat;
				float asScale;
			};
			LayoutValueType valueType;
		};

		struct LayoutAttributes {
			Box marginBox, positioningBox;

			HorizontalAlignment horizontalAlignment = HorizontalAlignment::Stretch;
			VerticalAlignment verticalAlignment = VerticalAlignment::Stretch;
		};

		class VirtualWindow : public Window {
		protected:
			std::string _title;
			CreateWindowFlags _createWindowFlags = 0;
			Window *_parent = nullptr;
			bool _closed = false;
			bool _shown = false;
			std::set<peff::RcObjectPtr<VirtualWindow>> _childWindows;
			int _x, _y;
			int _width, _height;

		public:
			CLENCH_NO_COPY_MOVE_METHODS(VirtualWindow);

			CLCWSAL_API VirtualWindow(
				WindowScope *windowScope,
				CreateWindowFlags flags,
				Window *parent,
				int x,
				int y,
				int width,
				int height);
			CLCWSAL_API virtual ~VirtualWindow();

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

			CLCWSAL_API virtual void pollEvents() override;

			CLCWSAL_API virtual void invalidate() override;

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

			CLCWSAL_API virtual const LayoutAttributes *getLayoutAttributes() const;
		};

		CLCWSAL_API void getAbsoluteOffsetToRootNativeWindow(Window *window, int &xOut, int &yOut);
		CLCWSAL_API Window *getRootNativeWindow(Window *childWindow);
		CLCWSAL_API void calcWindowLayout(
			const LayoutAttributes *layoutAttributes,
			int parentWindowX,
			int parentWindowY,
			int parentWindowWidth,
			int parentWindowHeight,
			int windowX,
			int windowY,
			int windowWidth,
			int windowHeight,
			int &windowXOut,
			int &windowYOut,
			int &windowWidthOut,
			int &windowHeightOut);
	}
}

#endif
