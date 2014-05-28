#ifndef GETEXTURE_H
#define GETEXTURE_H

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
		m_pTexTurePtr=NULL;
		m_eTextureType=TEX_UNDEFINED;
	}
	
	~geTexture()
	{
	}
	
	//void reset()
	//{
	//}
	
	void setTexturePack(steTexturePacket* pTexTurePtr)		{	m_pTexTurePtr=pTexTurePtr;			}
	steTexturePacket* getTexturePack()	const					{	return m_pTexTurePtr;				}

	unsigned int getTextureID()	const
    {
        return (m_pTexTurePtr)?m_pTexTurePtr->iTextureID:0;
    }
    
	void setTextureType(ETEXTURE type)		{	m_eTextureType=type;	}
	ETEXTURE getTextureType() const				{	return m_eTextureType;	}
	
private:
	steTexturePacket* m_pTexTurePtr;     //must not delete this pointer
	ETEXTURE m_eTextureType;
};

#endif