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
    
    unsigned int getTotalTextureMemory()    {   return totalTextureMemory;   }
    
	void setMetaDataFolder(const char* metaFolder)
	{
		GX_STRCPY(metaDataFolder, metaFolder);
	}

	void LoadDefaultTextures();
	stTexturePacket* getGEARTexture1x1()	{	return texturePacket1x1;	}

private:
	std::vector<stTexturePacket*>	texturePacketList;
    unsigned int totalTextureMemory;

	char metaDataFolder[512];
	stTexturePacket* texturePacket1x1;
};