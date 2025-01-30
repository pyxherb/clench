#ifndef _CLENCH_GHAL_BACKEND_H_
#define _CLENCH_GHAL_BACKEND_H_

#include "except.h"
#include <memory>
#include <peff/containers/string.h>
#include <peff/containers/hashmap.h>
#include <peff/containers/set.h>
#include <peff/containers/list.h>
#include <optional>

namespace clench {
	namespace ghal {
		class Device;

		class Backend : public peff::RcObject {
		protected:
			virtual bool doInit() = 0;
			virtual bool doDeinit() = 0;

		public:
			peff::RcObjectPtr<peff::Alloc> selfAllocator;
			const char *backendId;
			bool isInited;

			CLENCH_NO_COPY_MOVE_METHODS(Backend);

			CLCGHAL_API Backend(const char *backendId, peff::Alloc *selfAllocator);
			CLCGHAL_API virtual ~Backend();

			virtual void dealloc() = 0;

			CLCGHAL_API virtual void onRefZero() noexcept override;

			bool init();
			bool deinit();

			[[nodiscard]] virtual base::ExceptionPtr createDevice(Device *&ghalDeviceOut) = 0;
		};

		class GHALError : public std::runtime_error {
		public:
			CLCGHAL_API GHALError(const char *msg);
			CLCGHAL_API virtual ~GHALError();
		};

		CLCGHAL_API extern peff::HashMap<std::string_view, peff::RcObjectPtr<Backend>> g_registeredBackends;

		CLCGHAL_API void registerBackend(Backend *backend);
		CLCGHAL_API void unregisterBackend(const char *id);
		CLCGHAL_API Backend *getBackend(const char *id);
		CLCGHAL_API std::optional<std::pair<bool, const char *>> scanAndInitRegisteredBackends();
		CLCGHAL_API std::optional<std::pair<bool, const char *>> deinitInitedRegisteredBackends();
		CLCGHAL_API bool initRegisteredBackend(const char *id);
		CLCGHAL_API bool deinitRegisteredBackend(const char *id);

		CLCGHAL_API void registerBuiltinBackends(peff::Alloc *selfAllocator);

		[[nodiscard]] CLCGHAL_API base::ExceptionPtr createDevice(Device *&ghalDeviceOut, const peff::List<std::string_view> &preferredBackendNames = peff::List<std::string_view>());
	}
}

#endif
