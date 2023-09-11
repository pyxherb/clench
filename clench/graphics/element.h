#ifndef _CLENCH_GRAPHICS_ELEMENT_H_
#define _CLENCH_GRAPHICS_ELEMENT_H_

#include "base.h"

namespace clench {
	namespace graphics {
		/// @brief Interface for all elements.
		class IElement {
		public:
			virtual inline ~IElement() {}

			virtual void setVisible(bool visible) = 0;
			virtual bool isVisible() = 0;

			virtual Eigen::Vector3f getPos() = 0;
			virtual void setPos(Eigen::Vector3f pos) = 0;

			virtual void draw() = 0;
		};

		class ITextured {
		public:
			virtual inline ~ITextured() {}
			virtual void setTexture(Texture* texture) = 0;
		};

		class ISizable {
		public:
			virtual inline ~ISizable() {}

			virtual Eigen::Vector2f getSize() = 0;
			virtual void setSize(Eigen::Vector2f size) = 0;
		};
	}
}

#endif
