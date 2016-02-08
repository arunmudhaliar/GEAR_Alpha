#pragma once

#include "basicIncludes.h"

struct DECLSPEC stTexturePacket
{
	stTexturePacket()
	{
		iTextureID	= NULL;
		iTextureName= NULL;
		iOpTextureName = NULL;
		bAlphaTex=false;
		
		m_cWidth=0;
		m_cHeight=0;
		m_cBpp=0;
	}
    
    void releaseTextureData()
    {
        if(iTextureID>0)
			glDeleteTextures(1, &iTextureID);
		iTextureID	= 0;
        bAlphaTex=false;
        m_cWidth=m_cHeight=m_cBpp=0;
    }
    
	~stTexturePacket()
	{
        releaseTextureData();
        
        if(iTextureName)
        {            
#if defined (LOG_DEBUG_ENGINE)
            DEBUG_PRINT("unloading texture : %s", iTextureName);
#endif
        }
        
		GX_DELETE_ARY(iTextureName);
		GX_DELETE_ARY(iOpTextureName);
	}
    
	unsigned int	iTextureID;
	char*	iTextureName;
	char*	iOpTextureName;
	bool	bAlphaTex;
	
	unsigned int	m_cWidth;
	unsigned int	m_cHeight;
	unsigned int	m_cBpp;
};