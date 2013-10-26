#include "gxDebug.h"

extern "C"
{
void DEBUG_PRINT(const char* format, ...)
{
	char buffer[1024];
	va_list v;
	va_start(v,format);
	int ret = vsprintf(buffer, format, v);
	printf("%s\n", buffer);
	va_end(v);
}
}