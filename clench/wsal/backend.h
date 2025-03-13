#ifndef _CLENCH_WSAL_BACKEND_H_
#define _CLENCH_WSAL_BACKEND_H_

#include "window.h"
#include <peff/containers/hashmap.h>
#include <peff/containers/string.h>
#include <string_view>
#include <optional>

namespace clench {
	namespace wsal {
		class Window;

		class Backend : public peff::RcObject {
		protected:
			virtual bool doInit() = 0;
			virtual bool doDeinit() = 0;

		public:
			peff::RcObjectPtr<peff::Alloc> selfAllocator;
			peff::RcObjectPtr<peff::Alloc> resourceAllocator;
			const char *backendId;
			bool isInited = false;

			CLENCH_NO_COPY_MOVE_METHODS(Backend);

			CLCWSAL_API Backend(const char *backendId, peff::Alloc *selfAllocator, peff::Alloc *resourceAllocator);
			CLCWSAL_API virtual ~Backend();

			CLCWSAL_API virtual void onRefZero() noexcept override;

			virtual void dealloc() = 0;

			bool init();
			bool deinit();

			virtual base::ExceptionPtr createWindow(
				CreateWindowFlags flags,
				Window *parent,
				int x, int y,
				int width, int height,
				Window *&windowOut) = 0;
			virtual void setMouseCapture(Window *window, Window *childWindow) = 0;
			virtual void releaseMouseCapture(Window *window, Window *childWindow) = 0;
		};

		using BackendPtr = std::unique_ptr<Backend, peff::DeallocableDeleter<Backend>>;

		typedef bool (*EnumBackendsProc)(void *userData, Backend *backend);

		class WSAL {
		public:
			peff::HashMap<std::string_view, BackendPtr> registeredBackends;

			CLCWSAL_API WSAL(peff::Alloc *allocator);

			[[nodiscard]] CLCWSAL_API bool addBuiltinBackends(peff::Alloc *allocator);

			[[nodiscard]] CLCWSAL_API bool registerBackend(Backend *backend);
			[[nodiscard]] CLCWSAL_API Backend *getBackend(const std::string_view &name);
			[[nodiscard]] CLCWSAL_API bool unregisterBackend(const std::string_view &name);
			CLCWSAL_API void enumBackends(void *userData, EnumBackendsProc enumProc);
			CLCWSAL_API size_t getBackendNum();
		};
	}
}

#endif
