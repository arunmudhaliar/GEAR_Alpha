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

	float getClipX()	{	return spriteClipX;	}
	float getClipY()	{	return spriteClipY;	}

	float getClipWidth()		{	return spriteClipWidth;	}
	float getClipHeight()	{	return spriteClipHeight;	}
	
	int getOrigWidth()	{	return (int)originalSize.x;	}
	int getOrigHeight()    {	return (int)originalSize.y;	}
	
    //bxRectf getClip()       {   return bxRectf(spriteClipX, spriteClipY, spriteClipWidth, spriteClipHeight);  }
	geVector2f getOffset()		{	return spriteOffset;	}

	void setOffset(float x, float y);	// (0, 0) origin, (-1, 1) left top
    
    void debugDraw(geVector2f* pos=NULL);
	
	virtual void transformationChanged(){}
    virtual void onSpritePreRender(){}
    virtual void onSpritePostRender(){}
    
    geTexture* getTexture()  {   return &textureObject; }
    
    void clearRenderFlag()          {   spriteRenderFlag=0;        }
    void setRenderFlag(int flag)    {  spriteRenderFlag|=flag;     }
    int getRenderFlag()             {   return spriteRenderFlag;   }
    bool isRenderFlag(int flags)    {   return ((spriteRenderFlag&flags)>0)?true:false;    }
	
    void setAlpha(float alpha)        {   this->alpha=alpha;     }
    float getAlpha()                  {   return alpha;    }
    
    void setRGB(float r, float g, float b)   {   spriteRGB[0]=r; spriteRGB[1]=g; spriteRGB[2]=b;    }
	float* getRGB()		{	return spriteRGB;	}
    void setVFlip();
    void setHFlip();
    
    void setAlphaThreshold(float threshold)   {   alphaThreshold=threshold;   }
    float getAlphaThreshokld()                {  return alphaThreshold;       }
    
    virtual void transformationChangedf()
    {    }

	void setBlendMode(EBLENDFLAG eFlag)		{	spriteBlendFlag=eFlag;	}
	EBLENDFLAG getBlendMode()				{	return spriteBlendFlag;	}

	void setPos(int x, int y)	{	setPosition(x, y, 0);	}

	float* getVertexBuffer()	{	return spriteVertexArray;	}
	float* getTexCoordBuffer()	{	return spriteTextureCoordinateArray;	}

	void copy(Sprite& sprite);

protected:
	geTexture textureObject;
	geVector2f originalSize;
	geVector2f spriteOffset;
	float spriteVertexArray[8];
	float spriteTextureCoordinateArray[8];
    float spriteClipX;
    float spriteClipY;
	float spriteClipWidth;
	float spriteClipHeight;
    int spriteRenderFlag;
    float alpha;
    float spriteRGB[3];
    float alphaThreshold;
	EBLENDFLAG spriteBlendFlag;
};