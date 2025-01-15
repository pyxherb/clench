#ifndef _CLENCH_GHAL_BACKENDS_OPENGL_VIEWS_H_
#define _CLENCH_GHAL_BACKENDS_OPENGL_VIEWS_H_

#include "backend.h"
#include <clench/ghal/views.h>

namespace clench {
	namespace ghal {
		class GLRenderTargetView : public RenderTargetView {
		public:
			GLuint frameBufferHandle;

			CLENCH_NO_COPY_MOVE_METHODS(GLRenderTargetView);

			CLCGHAL_API GLRenderTargetView(
				GHALDevice *ownerDevice,
				RenderTargetViewType type,
				GLuint frameBufferHandle);
			CLCGHAL_API virtual ~GLRenderTargetView();

			CLCGHAL_API static GLRenderTargetView *alloc(
				GHALDevice *ownerDevice,
				RenderTargetViewType type,
				GLuint frameBufferHandle
			);
		};

		class GLDepthStencilView : public DepthStencilView {
		public:
			GLuint frameBufferHandle;
			GLuint depthBufferHandle;
			GLuint stencilBufferHandle;

			CLENCH_NO_COPY_MOVE_METHODS(GLDepthStencilView);

			CLCGHAL_API GLDepthStencilView(
				GHALDevice *ownerDevice,
				DepthStencilViewType depthStencilViewType,
				GLuint frameBufferHandle,
				GLuint depthBufferHandle,
				GLuint stencilBufferHandle);
			CLCGHAL_API virtual ~GLDepthStencilView();

			CLCGHAL_API static GLDepthStencilView *alloc(
				GHALDevice *ownerDevice,
				DepthStencilViewType depthStencilViewType,
				GLuint frameBufferHandle,
				GLuint depthBufferHandle,
				GLuint stencilBufferHandle
			);
		};
	}
}

#endif
