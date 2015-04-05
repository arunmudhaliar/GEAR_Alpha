#ifndef GEDEFINES_H
#define GEDEFINES_H

#include <string.h>

#ifdef _WIN32
    #define STRCPY(dst, src)	strcpy_s(dst, sizeof(dst), src)
#else
    #define STRCPY(dst, src)	strcpy(dst, src)
#endif

#define SPRINTF(dst, format, ...)	sprintf_s(dst, sizeof(dst), format, ...)

#define GE_DELETE(x)		if(x){delete x; x=NULL;}
#define GE_DELETE_ARY(x)	if(x){delete [] x; x=NULL;}

#define GE_SWAP_INT(x1, x2)     { int t=x1; x1=x2; x2=t;    }
#define GE_SWAP_FLOAT(x1, x2)   { float t=x1; x1=x2; x2=t;    }

class util
{
public:
	static bool GE_IS_EXTENSION(const char* path, const char* ext)
	{
		const char* extension_begin=strrchr(path, '.');
		if(extension_begin==NULL) return NULL;
		return (strcmp(extension_begin, ext)==0);
	}
};

#endif