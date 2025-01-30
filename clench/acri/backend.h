#ifndef _CLENCH_ACRI_BACKEND_H_
#define _CLENCH_ACRI_BACKEND_H_

#include "basedefs.h"
#include <clench/ghal/device.h>
#include <peff/containers/hashmap.h>
#include <peff/containers/string.h>

namespace clench {
	namespace acri {
		class Device;

		class Backend : public peff::RcObject {
		public:
			peff::RcObjectPtr<peff::Alloc> selfAllocator;
			const char *name;

			CLCACRI_API Backend(const char *name, peff::Alloc *selfAllocator);
			CLCACRI_API virtual ~Backend();

			virtual void dealloc() = 0;

			CLCACRI_API virtual void onRefZero() noexcept override;

			virtual base::ExceptionPtr createDevice(
				ghal::Device *ghalDevice,
				peff::Alloc *selfAllocator,
				peff::Alloc *resourceAllocator,
				Device *&deviceOut) = 0;
		};

		CLCACRI_API extern peff::HashMap<std::string_view, peff::RcObjectPtr<Backend>> g_registeredBackends;

		CLCACRI_API bool registerBackend(Backend *backend);
		CLCACRI_API bool unregisterBackend(std::string_view name);
		CLCACRI_API bool registerBuiltinBackends(peff::Alloc *selfAllocator);
	}
}

#endif
