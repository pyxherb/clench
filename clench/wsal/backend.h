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
			peff::RcObjectPtr<peff::Alloc> selfAllocator, resourceAllocator;
			const char *backendId;
			bool isInited;

			CLENCH_NO_COPY_MOVE_METHODS(Backend);

			CLCWSAL_API Backend(const char *backendId, peff::Alloc *selfAllocator, peff::Alloc *resourceAllocator);
			CLCWSAL_API virtual ~Backend();

			CLCWSAL_API virtual void onRefZero() noexcept override;

			bool init();
			bool deinit();

			virtual Window *createWindow(CreateWindowFlags flags, Window *parent, int x, int y, int width, int height) = 0;
		};

		CLCWSAL_API extern peff::HashMap<std::string_view, peff::RcObjectPtr<Backend>> g_registeredWSALBackends;

		CLCWSAL_API void registerWSALBackend(Backend *backend);
		CLCWSAL_API void unregisterWSALBackend(const char *id);
		CLCWSAL_API Backend *getWSALBackend(const char *id);
		CLCWSAL_API std::optional<std::pair<bool, const char *>> scanAndInitRegisteredWSALBackends();
		CLCWSAL_API std::optional<std::pair<bool, const char *>> deinitInitedRegisteredWSALBackends();
		CLCWSAL_API bool initRegisteredWSALBackend(const char *id);
		CLCWSAL_API bool deinitRegisteredWSALBackend(const char *id);

		CLCWSAL_API bool registerBuiltinWSALBackends(peff::Alloc *selfAllocator, peff::Alloc *resourceAllocator);

		CLCWSAL_API Window *createWindow(
			CreateWindowFlags flags,
			Window *parent,
			int x,
			int y,
			int width,
			int height,
			const peff::List<std::string_view> &preferredBackendNames = peff::List<std::string_view>());
	}
}

#endif
