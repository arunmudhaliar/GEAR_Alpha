#pragma once

#include "../../../GEAREngine/src/GEARFirstInclude.h"


#define DllImport   __declspec( dllimport )
#define DllExport   __declspec( dllexport )

#ifdef GEAR_APPLE
    #include <OpenGL/gl.h>
    #include <OpenGL/glu.h>
    #include <OpenGL/glext.h>
#ifndef GL_MAJOR_VERSION
    #define GL_MAJOR_VERSION                  0x821B
#endif
#ifndef GL_MINOR_VERSION
    #define GL_MINOR_VERSION                  0x821C
#endif
#elif defined(GEAR_WINDOWS)
    #include <Windows.h>
    #include "glew.h"
    #include <gl/GL.h>
    #include <gl/GLU.h>
    #include "wglew.h"
#else
    #ifndef GL_GLEXT_PROTOTYPES
        #define GL_GLEXT_PROTOTYPES
    #endif
    #include <GLES/gl.h>
    #include <GLES/glext.h>
    #include <GLES2/gl2.h>
    #include <GLES2/gl2ext.h>
#endif

#include <memory.h>