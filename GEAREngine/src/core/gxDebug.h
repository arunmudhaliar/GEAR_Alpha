#ifndef GXDEBUG_H
#define GXDEBUG_H

#include <stdio.h>
#include <stdarg.h>

extern "C"
{
	void DEBUG_PRINT(const char* format, ...);
}
#endif