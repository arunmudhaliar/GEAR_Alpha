/*
 *  gxTexture.h
 *  BMXProRider
 *
 *  Created by arun on 16/08/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef GXTEXTURE_H
#define GXTEXTURE_H

#include "stTexturePacket.h"

class gxTexture
{
public:
	enum ETEXTURE
	{
		TEX_NORMAL,
		TEX_ALPHA,
		TEX_UNDEFINED
	};
	
	gxTexture()
	{
		m_pTexTurePtr=NULL;
		m_eTextureType=TEX_UNDEFINED;
	}
	
	~gxTexture()
	{
	}
	
	void reset()
	{
	}
	
	void setTexture(stTexturePacket* pTexTurePtr)		{	m_pTexTurePtr=pTexTurePtr;			}
	unsigned int getTextureID()	const
    {
        return (m_pTexTurePtr)?m_pTexTurePtr->iTextureID:0;
    }
    
	void setTextureType(ETEXTURE type)		{	m_eTextureType=type;	}
	ETEXTURE getTextureType() const				{	return m_eTextureType;	}
	
private:
	stTexturePacket* m_pTexTurePtr;     //must not delete this pointer
	ETEXTURE m_eTextureType;
};

#endif