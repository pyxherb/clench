#ifndef _CLENCH_GHAL_BACKEND_H_
#define _CLENCH_GHAL_BACKEND_H_

#include "basedefs.h"
#include <memory>
#include <peff/containers/string.h>
#include <peff/containers/hashmap.h>
#include <peff/containers/set.h>
#include <peff/containers/list.h>

namespace clench {
	namespace ghal {
		class GHALDevice;

		class GHALBackend : public peff::RcObject {
		public:
			peff::RcObjectPtr<peff::Alloc> selfAllocator;
			const char *backendId;

			CLENCH_NO_COPY_MOVE_METHODS(GHALBackend);

			CLCGHAL_API GHALBackend(const char* backendId, peff::Alloc *selfAllocator);
			CLCGHAL_API virtual ~GHALBackend();

			CLCGHAL_API virtual void onRefZero() noexcept override;

			virtual GHALDevice* createDevice() = 0;
		};

		class GHALError : public std::runtime_error {
		public:
			CLCGHAL_API GHALError(const char *msg);
			CLCGHAL_API virtual ~GHALError();
		};

		CLCGHAL_API extern peff::HashMap<std::string_view, peff::RcObjectPtr<GHALBackend>> g_registeredGHALBackends;

		CLCGHAL_API void registerGHALBackend(GHALBackend* backend);
		CLCGHAL_API void unregisterGHALBackend(const char* id);
		CLCGHAL_API GHALBackend* getGHALBackend(const char* id);

		CLCGHAL_API void registerBuiltinGHALBackends(peff::Alloc *selfAllocator);

		CLCGHAL_API GHALDevice *createGHALDevice(const peff::List<std::string_view> &preferredBackendNames = peff::List<std::string_view>());
	}
}

#endif
