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
		m_pTexTurePtr=NULL;
		m_eTextureType=TEX_UNDEFINED;
		m_iFileCRC=0;
	}
	
	~gxTexture()
	{
	}
	
	void reset()
	{
	}
	
	void setTexture(stTexturePacket* pTexTurePtr)		{	m_pTexTurePtr=pTexTurePtr;			}

	const char* getTextureName()
	{
		if(m_pTexTurePtr)
			return m_pTexTurePtr->iTextureName;

		return NULL;
	}
	unsigned int getTextureID()	const
    {
        return (m_pTexTurePtr)?m_pTexTurePtr->iTextureID:0;
    }
    
	void setTextureType(ETEXTURE type)		{	m_eTextureType=type;	}
	ETEXTURE getTextureType() const			{	return m_eTextureType;	}
	
	matrix4x4f* getTextureMatrix()			{	return &m_cMatrix;		}

private:
	stTexturePacket* m_pTexTurePtr;     //must not delete this pointer
	ETEXTURE m_eTextureType;
	matrix4x4f m_cMatrix;
	int m_iFileCRC;
};