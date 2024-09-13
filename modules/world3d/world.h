#ifndef _CLENCH_WORLD3D_WORLD_H_
#define _CLENCH_WORLD3D_WORLD_H_

#include <clench/ghal.h>

namespace clench {
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
			utils::RcObjectPtr<ghal::Texture2D> renderOutputTexture;
			World *world;
		};
	}
}

#endif
