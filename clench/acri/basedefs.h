#ifndef _CLENCH_ACRI_BASEDEFS_H_
#define _CLENCH_ACRI_BASEDEFS_H_

#include <clench/ghal/basedefs.h>

#if CLENCH_DYNAMIC_LINK
	#if CLCACRI_BUILDING
		#define CLCACRI_API DLLEXPORT
	#else
		#define CLCACRI_API DLLIMPORT
	#endif
#else
	#define CLCACRI_API
#endif

#endif
