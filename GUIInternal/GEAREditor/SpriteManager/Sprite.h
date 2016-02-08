#pragma once

#include "../core/geTextureManager.h"
#include "../core/geTexture.h"
#include "../util/geVector2.h"
#ifndef GEAR2D
#include "../../../GEAREngine/src/hwShader/gxHWShader.h"
#endif
#include "../../../GEAREngine/src/core/transform.h"

class Sprite : public transform
{
public:
    
    enum ERENDER_FLAG
    {
        RENDER_NORMAL               = (1),
        RENDER_FORCE_NO_ALPHA       = (1<<1),
        RENDER_FORCE_NO_BLEND       = (1<<2),
        RENDER_FORCE_NO_MODULATION  = (1<<3),
        RENDER_FORCE_NO_TRANSFROM   = (1<<4),
        RENDER_V_FLIP               = (1<<5),
        RENDER_H_FLIP               = (1<<6)
    };
    
	enum EBLENDFLAG
	{
		BLEND_MULTIPLY,
		BLEND_ADDITIVE
	};

	Sprite();
	Sprite(Sprite& sprite);
	~Sprite();

	void loadTexture(CGETextureManager* textureManager, const char *pszFileName);
	
	virtual void draw(/*const matrix4x4f& parentTM, */geVector2f* pos=NULL){};
#ifndef GEAR2D
	virtual void draw(gxHWShader* shader){};
#endif

	void setClip(float clipX, float clipY, float width, float height);
	//void setClip(const bxRectf& rect)  {   setClip(rect.m_pos.x, rect.m_pos.y, rect.m_size.x, rect.m_size.y); }

	float getClipX()	{	return m_cClipX;	}
	float getClipY()	{	return m_cClipY;	}

	float getClipWidth()		{	return m_cClipWidth;	}
	float getClipHeight()	{	return m_cClipHeight;	}
	
	int getOrigWidth()	{	return (int)m_cOrigSize.x;	}
	int getOrigHeight()    {	return (int)m_cOrigSize.y;	}
	
    //bxRectf getClip()       {   return bxRectf(m_cClipX, m_cClipY, m_cClipWidth, m_cClipHeight);  }
	geVector2f getOffset()		{	return m_cOffset;	}

	void setOffset(float x, float y);	// (0, 0) origin, (-1, 1) left top
    
    void debugDraw(geVector2f* pos=NULL);
	
	virtual void transformationChanged(){}
    virtual void onSpritePreRender(){}
    virtual void onSpritePostRender(){}
    
    geTexture* getTexture()  {   return &m_cTexture; }
    
    void clearRenderFlag()          {   m_iRenderFlag=0;        }
    void setRenderFlag(int flag)    {  m_iRenderFlag|=flag;     }
    int getRenderFlag()             {   return m_iRenderFlag;   }
    bool isRenderFlag(int flags)    {   return ((m_iRenderFlag&flags)>0)?true:false;    }
	
    void setAlpha(float alpha)        {   m_fAlpha=alpha;     }
    float getAlpha()                  {   return m_fAlpha;    }
    
    void setRGB(float r, float g, float b)   {   m_fRGB[0]=r; m_fRGB[1]=g; m_fRGB[2]=b;    }
	float* getRGB()		{	return m_fRGB;	}
    void setVFlip();
    void setHFlip();
    
    void setAlphaThreshold(float threshold)   {   alphaThreshold=threshold;   }
    float getAlphaThreshokld()                {  return alphaThreshold;       }
    
    virtual void transformationChangedf()
    {    }

	void setBlendMode(EBLENDFLAG eFlag)		{	m_eBlendFlag=eFlag;	}
	EBLENDFLAG getBlendMode()				{	return m_eBlendFlag;	}

	void setPos(int x, int y)	{	setPosition(x, y, 0);	}

	float* getVertexBuffer()	{	return m_cszVertLst;	}
	float* getTexCoordBuffer()	{	return m_cszTexCoord;	}

	void copy(Sprite& sprite);

protected:
	geTexture m_cTexture;
	geVector2f m_cOrigSize;
	geVector2f m_cOffset;
	float m_cszVertLst[8];
	float m_cszTexCoord[8];
    float m_cClipX;
    float m_cClipY;
	float m_cClipWidth;
	float m_cClipHeight;
    int m_iRenderFlag;
    float m_fAlpha;
    float m_fRGB[3];
    float alphaThreshold;
	EBLENDFLAG m_eBlendFlag;
};