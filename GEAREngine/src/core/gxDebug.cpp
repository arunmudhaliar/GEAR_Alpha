#include "gxDebug.h"

#if defined(ANDROID)
#include <android/log.h>
#endif
//#define  LOG_TAG    "gearApp-native"
//#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
//#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
//#define  LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__))

extern "C"
{
void DEBUG_PRINT(const char* format, ...)
{
	char buffer[1024];
	va_list v;
	va_start(v,format);
	int ret = vsprintf(buffer, format, v);
#ifdef _WIN32
	printf("%s\n", buffer);
#elif defined(ANDROID)
	 __android_log_print(ANDROID_LOG_INFO,"gearApp-native", buffer);
#else
	printf("%s\n", buffer);
#endif
	va_end(v);
}
}