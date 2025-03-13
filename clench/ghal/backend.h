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

		typedef bool (*EnumBackendsProc)(void *userData, Backend *backend);

		using BackendPtr = std::unique_ptr<Backend, peff::DeallocableDeleter<Backend>>;

		class GHAL {
		public:
			peff::HashMap<std::string_view, BackendPtr> registeredBackends;

			CLCGHAL_API GHAL(peff::Alloc *allocator);

			[[nodiscard]] CLCGHAL_API bool addBuiltinBackends(peff::Alloc *allocator);

			[[nodiscard]] CLCGHAL_API bool registerBackend(Backend *backend);
			[[nodiscard]] CLCGHAL_API Backend *getBackend(const std::string_view &name);
			[[nodiscard]] CLCGHAL_API bool unregisterBackend(const std::string_view &name);
			CLCGHAL_API void enumBackends(void *userData, EnumBackendsProc enumProc);
			CLCGHAL_API size_t getBackendNum();
		};
	}
}

#endif
