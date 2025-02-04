#ifndef _CLENCH_AHAL_BACKEND_H_
#define _CLENCH_AHAL_BACKEND_H_

#include "except.h"
#include <memory>
#include <peff/containers/string.h>
#include <peff/containers/hashmap.h>
#include <peff/containers/set.h>
#include <peff/containers/list.h>
#include <string_view>
#include <optional>

namespace clench {
	namespace ahal {
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

			CLCAHAL_API Backend(const char *backendId, peff::Alloc *selfAllocator);
			CLCAHAL_API virtual ~Backend();

			virtual void dealloc() = 0;

			CLCAHAL_API virtual void onRefZero() noexcept override;

			bool init();
			bool deinit();

			[[nodiscard]] virtual base::ExceptionPtr createDevice(peff::Alloc *allocator, Device *&ahalDeviceOut) = 0;
		};

		class AHALError : public std::runtime_error {
		public:
			CLCAHAL_API AHALError(const char *msg);
			CLCAHAL_API virtual ~AHALError();
		};

		CLCAHAL_API extern peff::HashMap<std::string_view, peff::RcObjectPtr<Backend>> g_registeredBackends;

		CLCAHAL_API void registerBackend(Backend *backend);
		CLCAHAL_API void unregisterBackend(const char *id);
		CLCAHAL_API Backend *getBackend(const char *id);
		CLCAHAL_API std::optional<std::pair<bool, const char *>> scanAndInitRegisteredBackends();
		CLCAHAL_API std::optional<std::pair<bool, const char *>> deinitInitedRegisteredBackends();
		CLCAHAL_API bool initRegisteredBackend(const char *id);
		CLCAHAL_API bool deinitRegisteredBackend(const char *id);

		CLCAHAL_API void registerBuiltinBackends(peff::Alloc *selfAllocator);

		[[nodiscard]] CLCAHAL_API base::ExceptionPtr createDevice(Device *&ahalDeviceOut, const peff::List<std::string_view> &preferredBackendNames = peff::List<std::string_view>());
	}
}

#endif
