#ifndef __TGA_H__
#define __TGA_H__

//#pragma comment(lib, "Opengl32.lib")					//Link to OpenGL32.lib so we can use OpenGL stuff

//#include <windows.h>									// Standard windows header
#include <stdio.h>										// Standard I/O header 
#include <string.h>
#include <malloc.h>
//#include <gl\gl.h>										// Header for OpenGL32 library
//#include "Texture.h"


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
#ifdef _WIN32
__declspec( dllexport )bool LoadTGA(tgaTexture * texture, const char * filename);
#else
bool LoadTGA(tgaTexture * texture, const char * filename);
#endif
bool LoadUncompressedTGA(tgaTexture *, const char *, FILE *);	// Load an Uncompressed file
bool LoadCompressedTGA(tgaTexture *, const char *, FILE *);		// Load a Compressed file

#endif






