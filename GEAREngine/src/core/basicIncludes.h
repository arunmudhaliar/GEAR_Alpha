#pragma once

#include "../GEARFirstInclude.h"

#ifdef GEAR_WINDOWS
    #ifndef GEAR2D
        #if EXPORT_DLL
            #define DECLSPEC __declspec(dllexport)
        #else
            #define DECLSPEC __declspec(dllimport)
        #endif
    #else
        #define DECLSPEC
    #endif
    //#define DllImport   __declspec( dllimport )
    //#define DECLSPEC   __declspec( dllexport )
	#include <Windows.h>
	#include "../renderer/glew.h"
	#include <gl/gl.h>
#elif GEAR_ANDROID
	#define DllImport
	#define DECLSPEC// __attribute__((visibility("default")))
	#include <EGL/egl.h>
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
#elif GEAR_APPLE
    #ifdef DllImport
        #undef DllImport
    #endif
    #define DllImport
    #ifdef DECLSPEC
        #undef DECLSPEC
    #endif
    #define DECLSPEC// __attribute__((visibility("default")))
    #include <OpenGL/gl.h>
#endif


#include "gxDebug.h"
#include <cmath>

#if 0
#if defined(GEAR_WINDOWS) || defined(GEAR_APPLE)
#define USE_OPENAL
#elif defined(GEAR_ANDROID)
#define USE_OPENSL
#endif
#endif

#include <string>

#define ROUNDOFF(x) (x < 0.0001f && x > -0.0001f) ? 0.0f : x
#if GEAR_WINDOWS
	#define GX_STRCPY(dst, src)	strcpy_s(dst, sizeof(dst), src)
#else
	#define GX_STRCPY(dst, src)	strcpy(dst, src)
#endif

#define GX_DELETE(x)		if(x){delete x; x=NULL;}
#define GX_DELETE_ARY(x)	if(x){delete [] x; x=NULL;}

#define GX_SWAP_INT(x1, x2)     { int t=x1; x1=x2; x2=t;    }
#define GX_SWAP_FLOAT(x1, x2)   { float t=x1; x1=x2; x2=t;    }

#define GX_ABS(v) std::abs(v)

#define UNUSED(x)   (void)x
