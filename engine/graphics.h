#ifndef _ENGINE_GRAPHICS_H_
#define _ENGINE_GRAPHICS_H_

#include <clench/wsi.h>
#include <clench/rhi.h>
#include <memory>

namespace clench {
	namespace engine {
		extern std::unique_ptr<clench::rhi::RHIDevice> g_mainRhiDevice;
		extern std::unique_ptr<clench::rhi::RHIDeviceContext> g_mainRhiDeviceContext;
	}
}

#endif
