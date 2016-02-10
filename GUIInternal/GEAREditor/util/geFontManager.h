#pragma once

#include <vector>
#include "geDefines.h"
#include "../renderer/glincludes.h"
#include "../../../GEAREngine/src/util/gxFile.h"
#if defined (USE_ProgrammablePipeLine_test)
#include "../hwShader/gxHWShader.h"
#endif

#define GE_DEBUG_KERNING   0.0f

class geFont
{
public:
    enum ALIGNMENT
    {
        ALIGN_LEFT,
        ALIGN_RIGHT,
        ALIGN_CENTER,
        ALIGN_JUSTIFY
    };
    
#if defined (USE_ProgrammablePipeLine_test)
    geFont(gxHWShader* pFontShaderPtr, gxRenderer* renderer);
#else
    geFont();
#endif

    ~geFont();
    void reset();
    bool load(gxFile& file);
    void draw();
    
    int drawString(const char* str, int x, int y, int width_limit, bool bCentered=false, bool bShadowed=false, float elapsetime=0.0f);

    void drawOnBuffer(const char* str, int cx, int cy, unsigned int& nChar, short* vertexCoordBuffer, float* textureCoordBuffer, unsigned int& nLines, unsigned short* linebuffer, ALIGNMENT alignment) const;
    
    unsigned int getTexID() const {   return fontTextureID;    }
    
    unsigned short getLineHeight() const {   return lineHeight;   }
    void setLineHeight(unsigned short height)
    {
        lineHeight=height;
    }
    
    void setDeleteGLTexture(bool flag)  {   deleteFontTexture=flag;    }
    
    void setRGBA(float r, float g, float b, float a=1.0f)
    {
        colorRGBA[0]=r;        colorRGBA[1]=g;        colorRGBA[2]=b;        colorRGBA[3]=a;        
    }
    
    void setYOffset(float offy) {   fontYOffset=offy;    }

private:
	unsigned int loadBuffer(unsigned char* buffer, bool bAlpha, unsigned int width, unsigned int height, unsigned int bpp);

    void drawOnBuffer_left(const char* str, int cx, int cy, unsigned int& nChar, short* vertexCoordBuffer, float* textureCoordBuffer, unsigned int& nLines, unsigned short* linebuffer) const;
    bool canNeglectNextWord(const char* str, int cx, int nChar, int iCurIndex, float raster_pos_x) const;

    
    void drawOnBuffer_justify(const char* str, int cx, int cy, unsigned int& nChar, short* vertexCoordBuffer, float* textureCoordBuffer, unsigned int& nLines, unsigned short* linebuffer) const;
    bool calculateExtraSpaceOnRightSide(const char* str, int cx, int nChar, int iCurIndex, float& extraspace, int& nWords) const;
    
    void drawOnBuffer_center(const char* str, int cx, int cy, unsigned int& nChar, short* vertexCoordBuffer, float* textureCoordBuffer, unsigned int& nLines, unsigned short* linebuffer) const;
    bool calculateExtraSpaceOnBothSide(const char* str, int cx, int nChar, int iCurIndex, float& extraspace, int& nWords) const;
    
public:
    float calculateStringWidthInPixelTillNewLine(const char* str, int nChar, int iCurIndex);
	float getCharWidth(char charValue);

private:
    
    int bitmapWidth;
    int bitmapHeight;
    int baseChar;
    int totalChar;
    short spaceWidth;
    unsigned short lineHeight;
    
    short* charOffsetXArray;
    short* charOffsetYArray;
    unsigned short* charWidthArray;
    unsigned short* charHeightArray;
    short* charDistanceArray;
    float* textureCoordUArray;
    float* textureCoordVArray;
    float* textureCoordUArray_width;
    float* textureCoordUArray_height;
    
    unsigned int fontTextureID;
    
    short vertexCoordinateArray[12*256];
    float textureCoordinateArray[12*256];
    bool deleteFontTexture;
    float colorRGBA[4];
    float fontYOffset;   //for small adjustments
#if defined (USE_ProgrammablePipeLine_test)
    gxHWShader* fontShader;
	gxRenderer* renderer;
public:
	void setRenderer(gxRenderer* renderer)	{	this->renderer=renderer;	}
#endif
};

class geFontManager
{
public:
    geFontManager();
    ~geFontManager();
    
    void init();
    void reset(bool reload);
    geFont* loadFont(const char* filename);
    geFont* getFont(int index)                {   return fontList[index]; }

private:
    std::vector<geFont*> fontList;
#if defined (USE_ProgrammablePipeLine_test)
    gxHWShader fontHWShader;
	gxRenderer* renderer;
public:
	void setRenderer(gxRenderer* renderer);
#endif
    
public:
    static void InitializeFonts();
    static geFontManager g_cFontManager;
    static geFont* g_pFontArial10_84Ptr;
    static geFont* g_pFontArial10_80Ptr;
};