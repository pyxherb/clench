#ifndef _CLENCH_ACRI_BACKEND_H_
#define _CLENCH_ACRI_BACKEND_H_

#include "basedefs.h"
#include <clench/ghal/device.h>
#include <peff/containers/hashmap.h>
#include <peff/containers/string.h>

namespace clench {
	namespace acri {
		class ACRIDevice;

		class ACRIBackend : public peff::RcObject {
		public:
			peff::RcObjectPtr<peff::Alloc> selfAllocator;
			const char *name;

			CLCACRI_API ACRIBackend(const char *name, peff::Alloc *selfAllocator);
			CLCACRI_API virtual ~ACRIBackend();

			CLCACRI_API virtual void onRefZero() noexcept override;

			virtual ACRIDevice *createDevice(ghal::GHALDevice *ghalDevice, peff::Alloc *selfAllocator, peff::Alloc *resourceAllocator) = 0;
		};

		CLCACRI_API extern peff::HashMap<std::string_view, peff::RcObjectPtr<ACRIBackend>> g_registeredBackends;

		CLCACRI_API bool registerBackend(ACRIBackend *backend);
		CLCACRI_API bool unregisterBackend(std::string_view name);
		CLCACRI_API bool registerBuiltinBackends(peff::Alloc *selfAllocator);
	}
}

#endif
