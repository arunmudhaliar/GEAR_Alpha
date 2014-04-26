#ifndef BASICINCLUDES_H
#define BASICINCLUDES_H


#ifdef _WIN32
#define DllImport   __declspec( dllimport )
#define DllExport   __declspec( dllexport )
	#include <Windows.h>
	#include "../renderer/glew.h"
	#include <gl/gl.h>
#elif ANDROID
	#define DllImport
	#define DllExport __attribute__((visibility("default")))
	#include <EGL/egl.h>
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
#endif


#include "gxDebug.h"
//#include <gl/GLU.h>

#ifdef _WIN32
#define USE_OPENAL
#elif defined(ANDROID)
#define USE_OPENSL
#endif

#include <string.h>

#define ROUNDOFF(x) (x < 0.0001f && x > -0.0001f) ? 0.0f : x
#if _WIN32
	#define GX_STRCPY(dst, src)	strcpy_s(dst, sizeof(dst), src)
#else
	#define GX_STRCPY(dst, src)	strcpy(dst, src)
#endif

#define GX_DELETE(x)		if(x){delete x; x=NULL;}
#define GX_DELETE_ARY(x)	if(x){delete [] x; x=NULL;}

#define GX_SWAP_INT(x1, x2)     { int t=x1; x1=x2; x2=t;    }
#define GX_SWAP_FLOAT(x1, x2)   { float t=x1; x1=x2; x2=t;    }

#endif
