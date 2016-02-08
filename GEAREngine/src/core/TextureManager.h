#pragma once

#include "basicIncludes.h"

#include <string.h>
#include <vector>
#include "stTexturePacket.h"

class DECLSPEC CTextureManager
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

	void LoadDefaultTextures();
	stTexturePacket* getGEARTexture1x1()	{	return m_pGEARTexture1x1;	}

private:
	std::vector<stTexturePacket*>	iTexturePacket;
    unsigned int m_iTotalTextureMemory;

	char m_szMetaDataFolder[512];
	stTexturePacket* m_pGEARTexture1x1;
};