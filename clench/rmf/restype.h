#ifndef _CLENCH_RMF_RESTYPE_H_
#define _CLENCH_RMF_RESTYPE_H_

#include "basedefs.h"
#include <functional>
#include <unordered_map>
#include <string>
#include <string_view>

namespace clench {
	namespace rmf {
		using ResourceLoader = std::function<void *(const char *minorType, const char *path)>;
		using ResourceUnloader = std::function<void(void *resource)>;

		class ResourceType {
		public:
			CLCRMF_API virtual ~ResourceType();

			virtual void *loadResource(const char *minorType, const char *path) = 0;
			virtual void unloadResource(void *resource) = 0;
		};

		CLCRMF_API extern std::unordered_map<std::string, std::unique_ptr<ResourceType>> g_registeredResourceTypes;

		CLCRMF_API void registerResourceType(std::string_view majorType, ResourceTypeRegistry *registry);
		CLCRMF_API void unregisterResourceType(std::string_view majorType);
	}
}

#endif
