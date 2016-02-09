#include "Sprite.h"

Sprite::Sprite()/*:
transformf()*/
{
	m_cClipWidth=0;
	m_cClipHeight=0;
	m_cClipX=m_cClipY=0;
    
    clearRenderFlag();
    setRenderFlag(RENDER_NORMAL);
    
	setOffset(-1.0f, 1.0f);
    setAlpha(1.0f);
    setRGB(1.0f, 1.0f, 1.0f);
    setAlphaThreshold(1.0f);
	setBlendMode(BLEND_MULTIPLY);
}

Sprite::Sprite(Sprite& sprite)
{
	copy(sprite);
}

Sprite::~Sprite()
{
	
}

void Sprite::copy(Sprite& sprite)
{
	steTexturePacket *tp = sprite.getTexture()->getTexturePack(); 
	m_cTexture.setTexturePack(tp);
	
	if(tp->isAlphaTexure)
		m_cTexture.setTextureType(geTexture::TEX_ALPHA);
	else
		m_cTexture.setTextureType(geTexture::TEX_NORMAL);

	m_cOrigSize.set(sprite.getOrigWidth(), sprite.getOrigHeight());
	m_cOffset=sprite.getOffset();

	memcpy(m_cszVertLst, sprite.getVertexBuffer(), sizeof(m_cszVertLst));
	memcpy(m_cszTexCoord, sprite.getTexCoordBuffer(), sizeof(m_cszTexCoord));

	setClip(sprite.getClipX(), sprite.getClipY(), sprite.getClipWidth(), sprite.getClipHeight());
	setRenderFlag(sprite.getRenderFlag());
	setAlpha(sprite.getAlpha());
	setRGB(sprite.getRGB()[0], sprite.getRGB()[1], sprite.getRGB()[2]);
	setAlphaThreshold(sprite.getAlphaThreshokld());
	setBlendMode(sprite.getBlendMode());
}

void Sprite::loadTexture(CGETextureManager* textureManager, const char *pszFileName)
{
	steTexturePacket *tp = textureManager->LoadTexture(pszFileName); 
	m_cTexture.setTexturePack(tp);
	
	if(tp->isAlphaTexure)
		m_cTexture.setTextureType(geTexture::TEX_ALPHA);
	else
		m_cTexture.setTextureType(geTexture::TEX_NORMAL);
	
	m_cOrigSize.x = (float)tp->m_cWidth;
	m_cOrigSize.y = (float)tp->m_cHeight;
	
	setClip(0, 0, m_cOrigSize.x, m_cOrigSize.y);
}

void Sprite::setClip(float clipX, float clipY, float width, float height)
{
    m_cClipX=clipX;
    m_cClipY=clipY;
    
	m_cClipWidth=width;
	m_cClipHeight=height;
	
	float offX=(m_cOffset.x+1.0f)*0.5f;
	float offY=(-m_cOffset.y+1.0f)*0.5f;	
	
	const float vertLst[8] =
	{
		(width * (1.0f-offX)),		(height * (-offY)),
		(width * (-offX)),			(height * (-offY)),
		(width * (1.0f-offX)),		(height * (1.0f-offY)),
		(width * (-offX)),			(height * (1.0f-offY)),
	};
	
	memcpy(m_cszVertLst, vertLst, sizeof(vertLst));
	
	if(m_cOrigSize.x<=0.0f)	m_cOrigSize.x = 1.0f;
	if(m_cOrigSize.y<=0.0f)	m_cOrigSize.y = 1.0f;
	
	float min_x=(float)clipX/m_cOrigSize.x;
	float min_y=(float)clipY/m_cOrigSize.y;
	float max_x=(float)(clipX+width)/m_cOrigSize.x;
	float max_y=(float)(clipY+height)/m_cOrigSize.y;
	
	geVector2f aGLmin(min_x, min_y);
	geVector2f aGLmax(max_x, max_y);

	const float tex[8] =
	{
		(aGLmax.x)	, (aGLmin.y),
		(aGLmin.x)	, (aGLmin.y),
		(aGLmax.x)	, (aGLmax.y),
		(aGLmin.x)	, (aGLmax.y),
	};
	
	memcpy(m_cszTexCoord, tex, sizeof(tex));
}

void Sprite::setVFlip()
{
    GE_SWAP_FLOAT(m_cszTexCoord[0], m_cszTexCoord[4]);
    GE_SWAP_FLOAT(m_cszTexCoord[1], m_cszTexCoord[5]);
    GE_SWAP_FLOAT(m_cszTexCoord[2], m_cszTexCoord[6]);
    GE_SWAP_FLOAT(m_cszTexCoord[3], m_cszTexCoord[7]);
}

void Sprite::setHFlip()
{
    GE_SWAP_FLOAT(m_cszTexCoord[0], m_cszTexCoord[2]);
    GE_SWAP_FLOAT(m_cszTexCoord[1], m_cszTexCoord[3]);
    GE_SWAP_FLOAT(m_cszTexCoord[4], m_cszTexCoord[6]);
    GE_SWAP_FLOAT(m_cszTexCoord[5], m_cszTexCoord[7]);
}

void Sprite::setOffset(float x, float y)
{
	m_cOffset.x=x;
	m_cOffset.y=y;
}

void Sprite::debugDraw(geVector2f* pos)
{
    glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, m_cszVertLst);
    
    glPushMatrix();
	if(pos)
		glTranslatef(pos->x, pos->y, 0.0f);
	else
		glTranslatef(m[12], m[13], m[14]);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
	glPopMatrix();
    
   	glDisableClientState(GL_VERTEX_ARRAY);
}
