#pragma once

#include "stTexturePacket.h"
#include "matrix4x4f.h"
#include "GEARAsset.h"

class gxTexture : public GEARAsset
{
public:
	enum ETEXTURE
	{
		TEX_NORMAL,
		TEX_ALPHA,
		TEX_UNDEFINED
	};
	
	gxTexture():
		GEARAsset()
	{
		texture=NULL;
		textureType=TEX_UNDEFINED;
		textureFileCRC=0;
	}
	
	~gxTexture()
	{
	}
	
	void reset()
	{
	}
	
	void setTexture(stTexturePacket* pTexTurePtr)		{	texture=pTexTurePtr;			}

	const char* getTextureName()
	{
		if(texture)
			return texture->textureName;

		return NULL;
	}
	unsigned int getTextureID()	const
    {
        return (texture)?texture->textureID:0;
    }
    
	void setTextureType(ETEXTURE type)		{	textureType=type;	}
	ETEXTURE getTextureType() const			{	return textureType;	}
	
	matrix4x4f* getTextureMatrix()			{	return &textureMatrix;		}

private:
	stTexturePacket* texture;     //must not delete this pointer
	ETEXTURE textureType;
	matrix4x4f textureMatrix;
	int textureFileCRC;
};