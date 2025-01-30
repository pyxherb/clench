#ifndef _ENGINE_GRAPHICS_H_
#define _ENGINE_GRAPHICS_H_

#include <clench/wsal.h>
#include <clench/ghal.h>
#include <memory>

namespace clench {
	namespace editor {
		struct GraphicsContext {
			std::unique_ptr<ghal::Device> g_mainGhalDevice;
		};

		extern std::unique_ptr<ghal::Device, peff::DeallocableDeleter<ghal::Device>> g_mainGhalDevice;
	}
}

#endif
