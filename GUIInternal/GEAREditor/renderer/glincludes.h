/*
 *  glincludes.h
 *  waves
 *
 *  Created by arun on 10/01/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef GLINCLUDES_H
#define GLINCLUDES_H

//#define GL_DEBUG

#define DllImport   __declspec( dllimport )
#define DllExport   __declspec( dllexport )

#if defined(TARGET_IPHONE_SIMULATOR) || defined(TARGET_OS_IPHONE)
    #import <OpenGLES/ES1/gl.h>
    #import <OpenGLES/ES1/glext.h>
    #import <OpenGLES/ES2/gl.h>
    #import <OpenGLES/ES2/glext.h>
#elif defined(_WIN32)
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

#endif
