#pragma once

#include "steTexturePacket.h"

class geTexture
{
public:
	enum ETEXTURE
	{
		TEX_NORMAL,
		TEX_ALPHA,
		TEX_UNDEFINED
	};
	
	geTexture()
	{
		texture=NULL;
		textureType=TEX_UNDEFINED;
	}
	
	~geTexture()
	{
	}
	
	//void reset()
	//{
	//}
	
	void setTexturePack(steTexturePacket* pTexTurePtr)		{	texture=pTexTurePtr;			}
	steTexturePacket* getTexturePack()	const					{	return texture;				}

	unsigned int getTextureID()	const
    {
        return (texture)?texture->iTextureID:0;
    }
    
	void setTextureType(ETEXTURE type)		{	textureType=type;	}
	ETEXTURE getTextureType() const				{	return textureType;	}
	
private:
	steTexturePacket* texture;     //must not delete this pointer
	ETEXTURE textureType;
};