#include "gxUtil.h"

#ifdef GEAR_WINDOWS
	#if EXPORT_DLL
		char gxUtil::g_directoryPath[512];
		char gxUtil::g_filePath[512];
	#endif
#else
	char gxUtil::g_directoryPath[512];
	char gxUtil::g_filePath[512];
#endif