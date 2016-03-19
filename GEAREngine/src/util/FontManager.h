#pragma once

#include <vector>
#include <string>
#include "../renderer/gxRenderer.h"
#include "../util/gxFile.h"
#if defined (USE_ProgrammablePipeLine)
#include "../hwShader/gxHWShader.h"
#endif

#define DEBUG_KERNING   0.0f

class DECLSPEC gxFont
{
public:
    enum ALIGNMENT
    {
        ALIGN_LEFT,
        ALIGN_RIGHT,
        ALIGN_CENTER,
        ALIGN_JUSTIFY
    };
    
#if defined (USE_ProgrammablePipeLine)
    gxFont(gxHWShader* pFontShaderPtr, gxRenderer* renderer);
#else
    gxFont();
#endif

    ~gxFont();
    void reset();
    bool load(gxFile& file);
    void draw();
    
    int drawString(const std::string& str, int x, int y, int width_limit, bool bCentered=false, bool bShadowed=false, float elapsetime=0.0f);

    void drawOnBuffer(const std::string& str, int cx, int cy, unsigned int& nChar, short* vertexCoordBuffer, float* textureCoordBuffer, unsigned int& nLines, unsigned short* linebuffer, ALIGNMENT alignment) const;
    
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
#if defined (USE_ProgrammablePipeLine)
    gxHWShader* fontShader;
	gxRenderer* renderer;
public:
	gxRenderer* getRenderer()	{	return renderer;	}
	void setRenderer(gxRenderer* renderer)	{	this->renderer=renderer;	}
#endif
};

class DECLSPEC FontManager
{
public:
    FontManager();
    ~FontManager();
    
    void init();
    void reset(bool reload);
    gxFont* loadFont(const std::string& filename);
    gxFont* getFont(int index)                {   return fontList[index]; }

private:
    std::vector<gxFont*> fontList;
#if defined (USE_ProgrammablePipeLine)
    gxHWShader fontHWShader;
	gxRenderer* renderer;
    
public:
	void setRenderer(gxRenderer* renderer);
#endif
};