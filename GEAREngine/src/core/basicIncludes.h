#ifndef BASICINCLUDES_H
#define BASICINCLUDES_H

#define DllImport   __declspec( dllimport )
#define DllExport   __declspec( dllexport )

#include <Windows.h>
#include "../renderer/glew.h"
#include <gl/GL.h>
#include "gxDebug.h"
//#include <gl/GLU.h>


#include <string.h>

#define ROUNDOFF(x) (x < 0.0001f && x > -0.0001f) ? 0.0f : x
#define GX_STRCPY(dst, src)	strcpy_s(dst, sizeof(dst), src)

#define GX_DELETE(x)		if(x){delete x; x=NULL;}
#define GX_DELETE_ARY(x)	if(x){delete [] x; x=NULL;}

#define GX_SWAP_INT(x1, x2)     { int t=x1; x1=x2; x2=t;    }
#define GX_SWAP_FLOAT(x1, x2)   { float t=x1; x1=x2; x2=t;    }

#endif