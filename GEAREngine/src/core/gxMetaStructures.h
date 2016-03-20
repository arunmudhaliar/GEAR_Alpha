#pragma once
#include "../util/gxFile.h"
#include <sys/stat.h>

#if defined(GEAR_ANDROID) || defined(GEAR_APPLE)
	typedef long long __int64;
#endif

enum EMETA_FILETYPE
{
	eMetaUnknown		=	9000,
	eMetaTexture2D		=	9001,
	eMeta3DFile			=	9002,
	eMetaMaterial		=	9003,
};

enum ETEXTURE_2D_TYPE
{
	eTexture2D_Unknown,
	eTexture2D_RGB888,
	eTexture2D_RGBA8888,
	eTexture2D_RGB565,
	eTexture2D_RGBA5551,
	eTexture2D_BGR888,
	eTexture2D_BGRA8888,
};

struct stMetaHeader
{
	__int64 lastaccessed;		//information about the source file
	__int64 lastmodified;
	__int64 lastchanged;
	EMETA_FILETYPE filetype;
    
    void writeMetaHeader(const gxFile& file)
    {
        file.WriteBuffer((unsigned char*)this, sizeof(stMetaHeader));
    }
    
    struct stat readMetaHeader(const gxFile& file)
    {
        memset(this, 0, sizeof(stMetaHeader));
        file.ReadBuffer((unsigned char*)this, sizeof(stMetaHeader));
        
        struct stat fst;
        fst.st_atime = lastaccessed;
        fst.st_mtime = lastmodified;
        fst.st_ctime = lastchanged;
        
        return fst;
    }
    
    static int readFileStat(const std::string& filename, struct stat& fst)
    {
        memset(&fst, 0, sizeof(fst));
        return stat(filename.c_str(), &fst);
    }
};