#pragma once

#include <vector>
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
    gxFont(gxHWShader* pFontShaderPtr, gxRenderer* m_pRendererPtr);
#else
    gxFont();
#endif

    ~gxFont();
    void reset();
    bool load(gxFile& file);
    void draw();
    
    int drawString(const char* str, int x, int y, int width_limit, bool bCentered=false, bool bShadowed=false, float elapsetime=0.0f);

    void drawOnBuffer(const char* str, int cx, int cy, unsigned int& nChar, short* vertexCoordBuffer, float* textureCoordBuffer, unsigned int& nLines, unsigned short* linebuffer, ALIGNMENT alignment) const;
    
    unsigned int getTexID() const {   return m_iTexID;    }
    
    unsigned short getLineHeight() const {   return m_iLineHeight;   }
    void setLineHeight(unsigned short height)
    {
        m_iLineHeight=height;
    }
    
    void setDeleteGLTexture(bool flag)  {   m_bDeleteGLTexture=flag;    }
    
    void setRGBA(float r, float g, float b, float a=1.0f)
    {
        m_cszRGBA[0]=r;        m_cszRGBA[1]=g;        m_cszRGBA[2]=b;        m_cszRGBA[3]=a;        
    }
    
    void setYOffset(float offy) {   m_fYOffset=offy;    }

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
    
    int m_iBitmapWidth;
    int m_iBitmapHeight;
    int m_iBaseChar;
    int m_nChars;
    short m_iSpaceWidth;
    unsigned short m_iLineHeight;
    
    short* m_pszCharOffsetX;
    short* m_pszCharOffsetY;
    unsigned short* m_pszCharW;
    unsigned short* m_pszCharH;
    short* m_pszCharD;
    float* m_pszU;
    float* m_pszV;
    float* m_pszU_width;
    float* m_pszU_height;
    
    unsigned int m_iTexID;
    
    short m_cszVertCoordList[12*256];
    float m_cszTexCoordList[12*256];
    bool m_bDeleteGLTexture;
    float m_cszRGBA[4];
    float m_fYOffset;   //for small adjustments
#if defined (USE_ProgrammablePipeLine)
    gxHWShader* m_pFontShaderPtr;
	gxRenderer* m_pRendererPtr;
public:
	gxRenderer* getRenderer()	{	return m_pRendererPtr;	}
	void setRenderer(gxRenderer* renderer)	{	m_pRendererPtr=renderer;	}
#endif
};

class DECLSPEC FontManager
{
public:
    FontManager();
    ~FontManager();
    
    void init();
    void reset(bool reload);
    gxFont* loadFont(const char* filename);
    gxFont* getFont(int index)                {   return m_cvFontList[index]; }

private:
    std::vector<gxFont*> m_cvFontList;
#if defined (USE_ProgrammablePipeLine)
    gxHWShader m_cFontShader;
	gxRenderer* m_pRendererPtr;
public:
	void setRenderer(gxRenderer* renderer);
#endif
};