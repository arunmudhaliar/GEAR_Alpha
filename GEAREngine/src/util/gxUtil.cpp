#include "gxUtil.h"

#ifdef _WIN32
#if EXPORT_DLL
char gxUtil::g_directoryPath[512];
char gxUtil::g_filePath[512];
#endif
#endif