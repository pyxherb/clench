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

		class GHALBackend : public peff::RcObject {
		protected:
			virtual bool doInit() = 0;
			virtual bool doDeinit() = 0;

		public:
			peff::RcObjectPtr<peff::Alloc> selfAllocator;
			const char *backendId;
			bool isInited;

			CLENCH_NO_COPY_MOVE_METHODS(GHALBackend);

			CLCGHAL_API GHALBackend(const char *backendId, peff::Alloc *selfAllocator);
			CLCGHAL_API virtual ~GHALBackend();

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

		CLCGHAL_API extern peff::HashMap<std::string_view, peff::RcObjectPtr<GHALBackend>> g_registeredGHALBackends;

		CLCGHAL_API void registerGHALBackend(GHALBackend *backend);
		CLCGHAL_API void unregisterGHALBackend(const char *id);
		CLCGHAL_API GHALBackend *getGHALBackend(const char *id);
		CLCGHAL_API std::optional<std::pair<bool, const char *>> scanAndInitRegisteredGHALBackends();
		CLCGHAL_API std::optional<std::pair<bool, const char *>> deinitInitedRegisteredGHALBackends();
		CLCGHAL_API bool initRegisteredGHALBackend(const char *id);
		CLCGHAL_API bool deinitRegisteredGHALBackend(const char *id);

		CLCGHAL_API void registerBuiltinGHALBackends(peff::Alloc *selfAllocator);

		[[nodiscard]] CLCGHAL_API base::ExceptionPtr createDevice(Device *&ghalDeviceOut, const peff::List<std::string_view> &preferredBackendNames = peff::List<std::string_view>());
	}
}

#endif
