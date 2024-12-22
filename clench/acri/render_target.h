#ifndef _CLENCH_ACRI_CANVAS_H_
#define _CLENCH_ACRI_CANVAS_H_

#include "basedefs.h"
#include <clench/ghal/device.h>

namespace clench {
	namespace acri {
		class RenderTarget {
		public:
			CLENCH_NO_COPY_MOVE_METHODS(RenderTarget);

			ghal::GHALDeviceContext *deviceContext;
			ghal::RenderTargetView *renderTargetView;
		};
	}
}

#endif
