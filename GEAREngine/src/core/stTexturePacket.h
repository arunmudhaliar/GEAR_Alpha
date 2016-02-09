#pragma once

#include "basicIncludes.h"

struct DECLSPEC stTexturePacket
{
	stTexturePacket()
	{
		textureID	= NULL;
		textureName= NULL;
		alphaTextureName = NULL;
		isAlphaTexure=false;
		
		m_cWidth=0;
		m_cHeight=0;
		m_cBpp=0;
	}
    
    void releaseTextureData()
    {
        if(textureID>0)
			glDeleteTextures(1, &textureID);
		textureID	= 0;
        isAlphaTexure=false;
        m_cWidth=m_cHeight=m_cBpp=0;
    }
    
	~stTexturePacket()
	{
        releaseTextureData();
        
        if(textureName)
        {            
#if defined (LOG_DEBUG_ENGINE)
            DEBUG_PRINT("unloading texture : %s", textureName);
#endif
        }
        
		GX_DELETE_ARY(textureName);
		GX_DELETE_ARY(alphaTextureName);
	}
    
	unsigned int	textureID;
	char*	textureName;
	char*	alphaTextureName;
	bool	isAlphaTexure;
	
	unsigned int	m_cWidth;
	unsigned int	m_cHeight;
	unsigned int	m_cBpp;
};