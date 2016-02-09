#include "geTextureManager.h"
#ifndef GEAR2D
#include "../../../GEAREngine/src/util/pngLoader.h"
#else
#include "../../../../GEAR2D_Alpha/GEAR2D/app/pngLoaderfor2D.h"
#endif

CGETextureManager::CGETextureManager()
{
    totalTextureMemory=0;
}

CGETextureManager::~CGETextureManager()
{
	Reset();
}

steTexturePacket* CGETextureManager::LoadTexture(const char* aFileName, const char* aOpFileName)
{
	if(aFileName && aOpFileName)	//if both textures are there
	{
		int i=0;
		for(i=0;i<(int)texturePacketList.size();i++)
		{
			if(texturePacketList[i]->iTextureName && (strcmp(aFileName, texturePacketList[i]->iTextureName)==0))
			{
				if(texturePacketList[i]->iOpTextureName && (strcmp(aOpFileName, texturePacketList[i]->iOpTextureName)==0))
					break;
			}
		}
		if(i!=(int)texturePacketList.size())	return texturePacketList[i];	//if texture already exist in list
	}
	else 
	{
		if(aFileName)
		{
			int i=0;
			for(i=0;i<(int)texturePacketList.size();i++)
			{
				if(texturePacketList[i]->iTextureName && (strcmp(aFileName, texturePacketList[i]->iTextureName)==0))
				{
					if(!texturePacketList[i]->iOpTextureName)
						break;
				}
			}
			if(i!=(int)texturePacketList.size())	return texturePacketList[i];	//if texture already exist in list
		}//only diffuse
		else if(aOpFileName)
		{
			int i=0;
			for(i=0;i<(int)texturePacketList.size();i++)
			{
				if(texturePacketList[i]->iOpTextureName && (strcmp(aOpFileName, texturePacketList[i]->iOpTextureName)==0))
				{
					if(!texturePacketList[i]->iTextureName)
						break;
				}
			}
			if(i!=(int)texturePacketList.size())	return texturePacketList[i];	//if texture already exist in list
		}// only optex
	} // if there is only one tex

	steTexturePacket*	aNewTexturePacket	= new steTexturePacket();
	
	unsigned int texID=0;
	bool alpha_tex=false;

    //texture loading utility
	texID=read_png_file(aFileName, alpha_tex, aNewTexturePacket->m_cWidth, aNewTexturePacket->m_cHeight, aNewTexturePacket->m_cBpp);
    aNewTexturePacket->bAlphaTex=alpha_tex;

	if(texID==0)
	{
		printf("%s not found", aFileName);
		delete aNewTexturePacket;
		return NULL;
	}

    if(texID>0)
    {
        totalTextureMemory+=(unsigned int)(aNewTexturePacket->m_cWidth*aNewTexturePacket->m_cHeight*aNewTexturePacket->m_cBpp);
    }

	if(aFileName)
	{
		aNewTexturePacket->iTextureName = new char[strlen(aFileName)+1];
		strcpy(aNewTexturePacket->iTextureName, aFileName);
		aNewTexturePacket->iTextureName[strlen(aFileName)]	= '\0';
	}

	if(aOpFileName)
	{
		aNewTexturePacket->iOpTextureName = new char[strlen(aOpFileName)+1];
		strcpy(aNewTexturePacket->iOpTextureName, aOpFileName);
		aNewTexturePacket->iOpTextureName[strlen(aOpFileName)]	= '\0';
	}

	aNewTexturePacket->bAlphaTex	= alpha_tex;
	aNewTexturePacket->iTextureID	= texID;

	texturePacketList.push_back(aNewTexturePacket);
	
	return aNewTexturePacket;
}

void CGETextureManager::ReLoad()
{
}

void CGETextureManager::Reset()
{
	for(int i=0;i<texturePacketList.size();i++)
	{
		steTexturePacket* packet=texturePacketList[i];
		GE_DELETE(packet);
	}
	texturePacketList.clear();
    totalTextureMemory=0;
}