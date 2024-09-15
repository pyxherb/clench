#ifndef _CLENCH_WORLD3D_WORLD_H_
#define _CLENCH_WORLD3D_WORLD_H_

#include <clench/ghal.h>

namespace world3d {
	struct WorldContext {
	};

	class WorldObject {
	public:
		void render(WorldContext &context);
	};

	class World {
	};

	class WorldRenderer {
	public:
		clench::utils::RcObjectPtr<clench::ghal::Texture2D> renderOutputTexture;
		World *world;
	};
}

#endif
