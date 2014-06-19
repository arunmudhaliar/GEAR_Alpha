#ifndef GXDEBUG_H
#define GXDEBUG_H

#include <stdio.h>
#include <stdarg.h>
#include "basicIncludes.h"

#if defined(_WIN32) && defined(_DEBUG)
	#include <DbgHelp.h>
	#define LOG_GL_ERROR
#endif

extern "C" DECLSPEC void DEBUG_PRINT(const char* format, ...);



#ifdef LOG_GL_ERROR
//GL ERROR
extern "C" DECLSPEC const char* GetGLErrorName(GLenum errorValue);
extern "C" DECLSPEC void printCallStack();

//#pragma GCC diagnostic push
//#pragma GCC diagnostic ignored "-Wformat-security"
#define CHECK_GL_ERROR(fn)																			\
	do																								\
	{																								\
		fn;																							\
		GLenum error = glGetError();																\
		if (error != GL_NO_ERROR)																	\
		{																							\
			char errorMsg[1024];																	\
			_snprintf(errorMsg, 1024, #fn " failed with glError %s\n%s (%d)", GetGLErrorName(error), __FILE__, __LINE__);	\
			DEBUG_PRINT(errorMsg);																	\
			printCallStack();																		\
		}																							\
	} while(0)
//#pragma GCC diagnostic pop

#else
#define CHECK_GL_ERROR(fn) do { fn; } while(0)
#endif // GL_ERROR_CHECK_H_INCLUDED
//
#endif