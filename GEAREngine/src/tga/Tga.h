#pragma once
#include "../GEARFirstInclude.h"

#include <stdio.h>										// Standard I/O header
#include <string.h>
#if GEAR_APPLE
    #include <malloc/malloc.h>
#else
    #include <malloc.h>
#endif
#include "../core/basicIncludes.h"

typedef	struct									
{
	unsigned char	* imageData;									// Image Data (Up To 32 Bits)
	unsigned int	bpp;											// Image Color Depth In Bits Per Pixel
	unsigned int	width;											// Image Width
	unsigned int	height;											// Image Height
	unsigned int	texID;											// Texture ID Used To Select A Texture
	unsigned int	type;											// Image Type (GL_RGB, GL_RGBA)
} tgaTexture;	

typedef struct
{
	unsigned char Header[12];									// TGA File Header
} TGAHeader;


typedef struct
{
	unsigned char		header[6];								// First 6 Useful Bytes From The Header
	unsigned int		bytesPerPixel;							// Holds Number Of Bytes Per Pixel Used In The TGA File
	unsigned int		imageSize;								// Used To Store The Image Size When Setting Aside Ram
	unsigned int		temp;									// Temporary Variable
	unsigned int		type;	
	unsigned int		Height;									//Height of Image
	unsigned int		Width;									//Width ofImage
	unsigned int		Bpp;									// Bits Per Pixel
} TGA;


TGAHeader tgaheader;									// TGA header
TGA tga;												// TGA image data
unsigned char uTGAcompare[12] = {0,0,2, 0,0,0,0,0,0,0,0,0};	// Uncompressed TGA Header
unsigned char cTGAcompare[12] = {0,0,10,0,0,0,0,0,0,0,0,0};	// Compressed TGA Header
#ifdef GEAR_WINDOWS
DECLSPEC bool LoadTGA(tgaTexture * texture, const char * filename);
#else
bool LoadTGA(tgaTexture * texture, const char * filename);
#endif
bool LoadUncompressedTGA(tgaTexture *, const char *, FILE *);	// Load an Uncompressed file
bool LoadCompressedTGA(tgaTexture *, const char *, FILE *);		// Load a Compressed file