#ifndef _CLENCH_MODULES_WORLD3D_BASEDEFS_H_
#define _CLENCH_MODULES_WORLD3D_BASEDEFS_H_

#include <clench/base/basedefs.h>

#if CLENCH_DYNAMIC_LINK
	#if CLENCH_IS_USER_MODULE
		#define WORLD3D_API DLLEXPORT
	#else
		#define WORLD3D_API DLLIMPORT
	#endif
#else
	#define WORLD3D_API DLLEXPORT
#endif

#endif
