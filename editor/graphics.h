#ifndef _ENGINE_GRAPHICS_H_
#define _ENGINE_GRAPHICS_H_

#include <clench/wsal.h>
#include <clench/ghal.h>
#include <memory>

namespace clench {
	namespace editor {
		struct GraphicsContext {
			std::unique_ptr<ghal::GHALDevice> g_mainGhalDevice;
		};

		extern std::unique_ptr<ghal::GHALDevice, peff::DeallocableDeleter<ghal::GHALDevice>> g_mainGhalDevice;
	}
}

#endif
