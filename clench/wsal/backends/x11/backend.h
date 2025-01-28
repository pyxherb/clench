#ifndef _CLENCH_WSAL_BACKENDS_X11_BACKEND_H_
#define _CLENCH_WSAL_BACKENDS_X11_BACKEND_H_

#include "window.h"
#include <clench/wsal/backend.h>

namespace clench {
	namespace wsal {

		class X11Backend : public Backend {
		protected:
			CLCWSAL_API virtual bool doInit() override;
			CLCWSAL_API virtual bool doDeinit() override;

			CLCWSAL_API static int _x11ErrorHandler(Display *display, XErrorEvent *error);

		public:
			peff::Map<X11WindowHandle, Window *> handleToWindowMap;
			peff::Map<unsigned int, KeyboardKeyCode> builtNativeKeyMap;
			base::ExceptionPtr lastExceptionPtr;

			struct CheckedMutex {
				X11Backend *backend;
				std::mutex mutex;

				CLENCH_FORCEINLINE CheckedMutex(X11Backend *backend) : backend(backend) {
				}

				CLENCH_FORCEINLINE void lock() {
					mutex.lock();
				}

				CLENCH_FORCEINLINE void unlock() {
					backend->lastExceptionPtr.unwrap();
					mutex.unlock();
				}
			};
			CheckedMutex operationMutex;

			CLENCH_NO_COPY_MOVE_METHODS(X11Backend);

			CLCWSAL_API X11Backend(
				peff::Alloc *selfAllocator,
				peff::Alloc *resourceAllocator);
			CLCWSAL_API virtual ~X11Backend();

			CLCWSAL_API virtual void dealloc() override;

			CLCWSAL_API virtual base::ExceptionPtr createWindow(
				CreateWindowFlags flags,
				Window *parent,
				int x,
				int y,
				int width,
				int height,
				Window *&windowOut) override;
			CLCWSAL_API virtual void setMouseCapture(Window *window, Window *childWindow) override;
			CLCWSAL_API virtual void releaseMouseCapture(Window *window, Window *childWindow) override;

			CLCWSAL_API static X11Backend *alloc(
				peff::Alloc *selfAllocator,
				peff::Alloc *resourceAllocator);
		};

		extern X11Backend *g_x11Backend;
	}
}

#endif
