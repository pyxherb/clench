#ifndef _CLENCH_GHAL_BACKENDS_OPENGL_TEXTURE_H_
#define _CLENCH_GHAL_BACKENDS_OPENGL_TEXTURE_H_

#include "backend.h"
#include <clench/ghal/texture.h>

namespace clench {
	namespace ghal {
		class GLTexture1D : public Texture1D {
		public:
			GLuint textureHandle = UINT_MAX;

			CLENCH_NO_COPY_MOVE_METHODS(GLTexture1D);

			CLCGHAL_API GLTexture1D(
				GHALDevice *ownerDevice,
				const Texture1DDesc &textureDesc,
				GLuint textureHandle);
			CLCGHAL_API virtual ~GLTexture1D();

			CLCGHAL_API virtual void dealloc() override;

			CLCGHAL_API static GLTexture1D *alloc(
				GHALDevice *ownedDevice,
				const Texture1DDesc &textureDesc,
				GLuint textureHandle);
		};

		class GLTexture2D : public Texture2D {
		public:
			GLuint textureHandle = UINT_MAX;

			CLENCH_NO_COPY_MOVE_METHODS(GLTexture2D);

			CLCGHAL_API GLTexture2D(
				GHALDevice *ownerDevice,
				const Texture2DDesc &textureDesc,
				GLuint textureHandle);
			CLCGHAL_API virtual ~GLTexture2D();

			CLCGHAL_API virtual void dealloc() override;

			CLCGHAL_API static GLTexture2D *alloc(
				GHALDevice *ownerDevice,
				const Texture2DDesc &textureDesc,
				GLuint textureHandle);
		};

		class GLTexture3D : public Texture3D {
		public:
			GLuint textureHandle = UINT_MAX;

			CLENCH_NO_COPY_MOVE_METHODS(GLTexture3D);

			CLCGHAL_API GLTexture3D(
				GHALDevice *ownerDevice,
				const Texture3DDesc &textureDesc,
				GLuint textureHandle);
			CLCGHAL_API virtual ~GLTexture3D();

			CLCGHAL_API virtual void dealloc() override;

			CLCGHAL_API static GLTexture3D *alloc(
				GHALDevice *ownerDevice,
				const Texture3DDesc &textureDesc,
				GLuint textureHandle);
		};
	}
}

#endif
