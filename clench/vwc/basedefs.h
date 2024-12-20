#ifndef _CLENCH_VWC_BASEDEFS_H_
#define _CLENCH_VWC_BASEDEFS_H_

#include <clench/wsal.h>
#include <clench/ghal.h>

#if CLENCH_DYNAMIC_LINK
	#ifdef CLCVWC_BUILDING
		#define CLCVWC_API CLENCH_DLLEXPORT
	#else
		#define CLCVWC_API CLENCH_DLLIMPORT
	#endif
#else
	#define CLCVWC_API
#endif

#endif
