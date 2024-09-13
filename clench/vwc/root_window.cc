#include "root_window.h"

using namespace clench;
using namespace clench::vwc;

CLCVWC_API RootWindow::RootWindow(
	wsal::CreateWindowFlags createWindowFlags,
	ghal::GHALDevice *ghalDevice,
	NativeWindow *parent,
	int x,
	int y,
	int width,
	int height)
	: NativeWindow(
		  createWindowFlags,
		  parent,
		  x,
		  y,
		  width,
		  height),
	  ghalDevice(ghalDevice) {
	ghalDeviceContext = std::unique_ptr<ghal::GHALDeviceContext>(ghalDevice->createDeviceContextForWindow(this));
}

CLCVWC_API RootWindow::~RootWindow() {
}

CLCVWC_API void RootWindow::addChildWindow(Window *window) {
	childWindows.insert(window);
}

CLCVWC_API void RootWindow::removeChildWindow(Window *window) {
	childWindows.erase(window);
}

CLCVWC_API bool RootWindow::hasChildWindow(Window *window) const {
	return childWindows.count(window);
}

CLCVWC_API void RootWindow::onResize(int width, int height) {
	ghalDeviceContext->onResize(width, height);

	wsal::WindowProperties windowProperties;

	for (auto i : childWindows) {
		i->getWindowProperties(windowProperties);

		if (windowProperties.isNative)
			continue;

		if (const wsal::LayoutAttributes *layoutAttribs =
				((wsal::VirtualWindow *)i.get())->getLayoutAttributes();
			layoutAttribs) {
			int windowX, windowY, windowWidth, windowHeight;
			int newX, newY, newWidth, newHeight;

			i->getPos(windowX, windowY);
			i->getSize(windowWidth, windowHeight);

			wsal::calcWindowLayout(
				layoutAttribs,
				0, 0,
				width, height,
				windowX, windowY,
				windowWidth, windowHeight,
				newX, newY,
				newWidth, newHeight);

			i->setPos(newX, newY);
			i->setSize(newWidth, newHeight);
		}
	}
}

CLCVWC_API void RootWindow::onMove(int x, int y) {
}

CLCVWC_API bool RootWindow::onClose() {
	return false;
}

CLCVWC_API void RootWindow::onKeyDown(wsal::KeyboardKeyCode keyCode) {
}

CLCVWC_API void RootWindow::onKeyUp(wsal::KeyboardKeyCode keyCode) {
}

CLCVWC_API void RootWindow::onMouseButtonPress(wsal::MouseButton button, int x, int y) {
	if (auto capturedWindow = wsal::getMouseCapture(); capturedWindow) {
		int xOffset, yOffset;
		wsal::getAbsoluteOffsetToRootNativeWindow(capturedWindow, xOffset, yOffset);

		capturedWindow->onMouseButtonPress(button, x - xOffset, y - yOffset);
	} else {
		std::map<Window *, std::pair<int, int>> childWindows;

		findWindowsAtPos(x, y, childWindows);

		for (auto i : childWindows) {
			i.first->onMouseButtonPress(button, i.second.first, i.second.second);
		}
	}
}

CLCVWC_API void RootWindow::onMouseButtonRelease(wsal::MouseButton button, int x, int y) {
	if (auto capturedWindow = wsal::getMouseCapture(); capturedWindow) {
		int xOffset, yOffset;
		wsal::getAbsoluteOffsetToRootNativeWindow(capturedWindow, xOffset, yOffset);

		capturedWindow->onMouseButtonRelease(button, x - xOffset, y - yOffset);
	} else {
		std::map<Window *, std::pair<int, int>> childWindows;

		findWindowsAtPos(x, y, childWindows);

		for (auto i : childWindows) {
			i.first->onMouseButtonRelease(button, i.second.first, i.second.second);
		}
	}
}

CLCVWC_API void RootWindow::onMouseHover(int x, int y) {
	if (auto capturedWindow = wsal::getMouseCapture(); capturedWindow) {
		int xOffset, yOffset;
		wsal::getAbsoluteOffsetToRootNativeWindow(capturedWindow, xOffset, yOffset);

		capturedWindow->onMouseHover(x - xOffset, y - yOffset);
	} else {
		std::map<Window *, std::pair<int, int>> childWindows;

		findWindowsAtPos(x, y, childWindows);

		for (auto i : childWindows) {
			i.first->onMouseHover(i.second.first, i.second.second);
		}
	}
}

CLCVWC_API void RootWindow::onMouseLeave() {
	/*
	for (auto i : hoveredChildWindows) {
		i->onMouseLeave();
	}

	hoveredChildWindows.clear();*/
}

CLCVWC_API void RootWindow::onMouseMove(int x, int y) {
	if (auto capturedWindow = wsal::getMouseCapture(); capturedWindow) {
		int xOffset, yOffset;
		wsal::getAbsoluteOffsetToRootNativeWindow(capturedWindow, xOffset, yOffset);

		int width, height;

		capturedWindow->getSize(width, height);

		if ((x >= xOffset) &&
			(x < xOffset + width) &&
			(y >= yOffset) &&
			(y < yOffset + height)) {
			if (!hoveredChildWindows.count(capturedWindow)) {
				capturedWindow->onMouseHover(x - xOffset, y - yOffset);
				hoveredChildWindows.insert(capturedWindow);
			} else {
				capturedWindow->onMouseMove(x - xOffset, y - yOffset);
			}
		} else {
			if (hoveredChildWindows.count(capturedWindow)) {
				capturedWindow->onMouseLeave();
				hoveredChildWindows.erase(capturedWindow);
			}
		}
	} else {
		std::map<Window *, std::pair<int, int>> childWindows;

		findWindowsAtPos(x, y, childWindows);

		{
			std::set<Window *> leftWindows;
			for (auto i : hoveredChildWindows) {
				if (!childWindows.count(i)) {
					leftWindows.insert(i);
				}
			}

			for (auto i : leftWindows) {
				i->onMouseLeave();
				hoveredChildWindows.erase(i);
			}
		}

		for (auto i : childWindows) {
			if (!hoveredChildWindows.count(i.first)) {
				i.first->onMouseHover(x, y);
				hoveredChildWindows.insert(i.first);
			} else
				i.first->onMouseMove(i.second.first, i.second.second);
		}
	}
}

CLCVWC_API void RootWindow::onDraw() {
	ghalDeviceContext->begin();

	int width, height;
	getSize(width, height);

	ghalDeviceContext->setViewport(0, 0, width, height, 0.0f, 1.0f);

	for (auto i : childWindows) {
		int subwindowX, subwindowY;
		int subwindowWidth, subwindowHeight;

		i->getPos(subwindowX, subwindowY);
		i->getSize(subwindowWidth, subwindowHeight);

		ghalDeviceContext->setViewport(
			subwindowX,
			subwindowY,
			subwindowWidth,
			subwindowHeight,
			0.0f,
			1.0f);

		i->onDraw();
	}

	ghalDeviceContext->end();
	ghalDeviceContext->present();
}

CLCVWC_API void RootWindow::redrawChildWindows() {
	for (auto i : childWindows) {
		i->onExpose();
	}
}

CLCVWC_API void RootWindow::findWindowsAtPos(int x, int y, std::map<Window *, std::pair<int, int>> &childWindowsOut) {
	for (auto i : childWindows) {
		int windowX, windowY, windowWidth, windowHeight;
		wsal::getAbsoluteOffsetToRootNativeWindow(i.get(), windowX, windowY);
		i->getSize(windowWidth, windowHeight);

		if ((x >= windowX) &&
			(y >= windowY) &&
			(x < windowX + windowWidth) &&
			(y < windowY + windowHeight))
			childWindowsOut[i.get()] = { x - windowX, y - windowY };
	}
}
