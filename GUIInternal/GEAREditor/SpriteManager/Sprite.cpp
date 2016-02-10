#include "Sprite.h"

Sprite::Sprite()/*:
transformf()*/
{
	spriteClipWidth=0;
	spriteClipHeight=0;
	spriteClipX=spriteClipY=0;
    
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
	textureObject.setTexturePack(tp);
	
	if(tp->isAlphaTexure)
		textureObject.setTextureType(geTexture::TEX_ALPHA);
	else
		textureObject.setTextureType(geTexture::TEX_NORMAL);

	originalSize.set(sprite.getOrigWidth(), sprite.getOrigHeight());
	spriteOffset=sprite.getOffset();

	memcpy(spriteVertexArray, sprite.getVertexBuffer(), sizeof(spriteVertexArray));
	memcpy(spriteTextureCoordinateArray, sprite.getTexCoordBuffer(), sizeof(spriteTextureCoordinateArray));

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
	textureObject.setTexturePack(tp);
	
	if(tp->isAlphaTexure)
		textureObject.setTextureType(geTexture::TEX_ALPHA);
	else
		textureObject.setTextureType(geTexture::TEX_NORMAL);
	
	originalSize.x = (float)tp->m_cWidth;
	originalSize.y = (float)tp->m_cHeight;
	
	setClip(0, 0, originalSize.x, originalSize.y);
}

void Sprite::setClip(float clipX, float clipY, float width, float height)
{
    spriteClipX=clipX;
    spriteClipY=clipY;
    
	spriteClipWidth=width;
	spriteClipHeight=height;
	
	float offX=(spriteOffset.x+1.0f)*0.5f;
	float offY=(-spriteOffset.y+1.0f)*0.5f;	
	
	const float vertLst[8] =
	{
		(width * (1.0f-offX)),		(height * (-offY)),
		(width * (-offX)),			(height * (-offY)),
		(width * (1.0f-offX)),		(height * (1.0f-offY)),
		(width * (-offX)),			(height * (1.0f-offY)),
	};
	
	memcpy(spriteVertexArray, vertLst, sizeof(vertLst));
	
	if(originalSize.x<=0.0f)	originalSize.x = 1.0f;
	if(originalSize.y<=0.0f)	originalSize.y = 1.0f;
	
	float min_x=(float)clipX/originalSize.x;
	float min_y=(float)clipY/originalSize.y;
	float max_x=(float)(clipX+width)/originalSize.x;
	float max_y=(float)(clipY+height)/originalSize.y;
	
	geVector2f aGLmin(min_x, min_y);
	geVector2f aGLmax(max_x, max_y);

	const float tex[8] =
	{
		(aGLmax.x)	, (aGLmin.y),
		(aGLmin.x)	, (aGLmin.y),
		(aGLmax.x)	, (aGLmax.y),
		(aGLmin.x)	, (aGLmax.y),
	};
	
	memcpy(spriteTextureCoordinateArray, tex, sizeof(tex));
}

void Sprite::setVFlip()
{
    GE_SWAP_FLOAT(spriteTextureCoordinateArray[0], spriteTextureCoordinateArray[4]);
    GE_SWAP_FLOAT(spriteTextureCoordinateArray[1], spriteTextureCoordinateArray[5]);
    GE_SWAP_FLOAT(spriteTextureCoordinateArray[2], spriteTextureCoordinateArray[6]);
    GE_SWAP_FLOAT(spriteTextureCoordinateArray[3], spriteTextureCoordinateArray[7]);
}

void Sprite::setHFlip()
{
    GE_SWAP_FLOAT(spriteTextureCoordinateArray[0], spriteTextureCoordinateArray[2]);
    GE_SWAP_FLOAT(spriteTextureCoordinateArray[1], spriteTextureCoordinateArray[3]);
    GE_SWAP_FLOAT(spriteTextureCoordinateArray[4], spriteTextureCoordinateArray[6]);
    GE_SWAP_FLOAT(spriteTextureCoordinateArray[5], spriteTextureCoordinateArray[7]);
}

void Sprite::setOffset(float x, float y)
{
	spriteOffset.x=x;
	spriteOffset.y=y;
}

void Sprite::debugDraw(geVector2f* pos)
{
    glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, spriteVertexArray);
    
    glPushMatrix();
	if(pos)
		glTranslatef(pos->x, pos->y, 0.0f);
	else
		glTranslatef(m[12], m[13], m[14]);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
	glPopMatrix();
    
   	glDisableClientState(GL_VERTEX_ARRAY);
}
