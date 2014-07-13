#ifndef GXMETASTRUCTURES_H
#define GXMETASTRUCTURES_H

#ifdef ANDROID
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
};

#endif