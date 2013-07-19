#ifndef _TEXTUREMANAGER_H_
#define _TEXTUREMANAGER_H_

//#include "../util/objC_util.h"

#include "basicIncludes.h"

#include <string.h>
#include <vector>
#include "stTexturePacket.h"

class CTextureManager
{
public:
	CTextureManager();
	~CTextureManager();

	stTexturePacket*	LoadTexture(const char* aFileName);
	void	Reset();	
    void    ReLoad();   //reload all textures from physical memory
    
    unsigned int getTotalTextureMemory()    {   return m_iTotalTextureMemory;   }
    
	void setMetaDataFolder(const char* metaFolder)
	{
		GX_STRCPY(m_szMetaDataFolder, metaFolder);
	}

private:
	std::vector<stTexturePacket*>	iTexturePacket;
    unsigned int m_iTotalTextureMemory;

	char m_szMetaDataFolder[512];
};

#endif