#ifndef _CLENCH_MODULES_DFTBOOT_BASEDEFS_H_
#define _CLENCH_MODULES_DFTBOOT_BASEDEFS_H_

#include <clench/base/basedefs.h>

#if CLENCH_DYNAMIC_LINK
	#if CLENCH_IS_USER_MODULE
		#define DFTBOOT_API DLLEXPORT
	#else
		#define DFTBOOT_API DLLIMPORT
	#endif
#else
	#define DFTBOOT_API
#endif

#endif
