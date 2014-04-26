#include "TextureManager.h"
#include "gxMetaStructures.h"
#include "../util/gxFile.h"

#if defined(WIN32)
#include "../util/pngLoader.h"
#endif

unsigned int read_texture2D_from_metafile(const char* file_name, bool& bAlpha, unsigned int& cx, unsigned int& cy, unsigned int& bpp);

CTextureManager::CTextureManager()
{
    m_iTotalTextureMemory=0;
	m_pGEARTexture1x1=NULL;
}

CTextureManager::~CTextureManager()
{
	Reset();
#if defined (LOG_DEBUG_ENGINE)
    DEBUG_PRINT("Texture Manager Destroyed...");
#endif
}

void CTextureManager::LoadDefaultTextures()
{
	stTexturePacket*	whiteTexPack	= new stTexturePacket();

	unsigned char tex1x1[3];
	whiteTexPack->m_cWidth=1;
	whiteTexPack->m_cHeight=1;
	whiteTexPack->m_cBpp=3;
	memset(tex1x1, 128, sizeof(tex1x1));

    //texture loading utility
    glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &whiteTexPack->iTextureID);
	glBindTexture(GL_TEXTURE_2D, whiteTexPack->iTextureID);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, tex1x1);
    glDisable(GL_TEXTURE_2D);

	whiteTexPack->bAlphaTex=false;
    if(whiteTexPack->iTextureID>0)
    {
        m_iTotalTextureMemory+=(unsigned int)(whiteTexPack->m_cWidth*whiteTexPack->m_cHeight*whiteTexPack->m_cBpp);
    }

	whiteTexPack->iTextureName = new char[strlen("gearWhiteTex1x1")+1];
	strcpy(whiteTexPack->iTextureName, "gearWhiteTex1x1");
	whiteTexPack->iTextureName[strlen("gearWhiteTex1x1")]	= '\0';

	m_pGEARTexture1x1 = whiteTexPack;
	iTexturePacket.push_back(whiteTexPack);
}

stTexturePacket* CTextureManager::LoadTexture(const char* aFileName)
{
	if(aFileName)
	{
		int i=0;
		for(i=0;i<(int)iTexturePacket.size();i++)
		{
			if(iTexturePacket[i]->iTextureName && (strcmp(aFileName, iTexturePacket[i]->iTextureName)==0))
			{
				if(!iTexturePacket[i]->iOpTextureName)
					break;
			}
		}
		if(i!=(int)iTexturePacket.size())
			return iTexturePacket[i];	//if texture already exist in list
	}//only diffuse


	stTexturePacket*	aNewTexturePacket	= new stTexturePacket();
	
	unsigned int texID=0;
	bool alpha_tex=false;

    //texture loading utility
    glEnable(GL_TEXTURE_2D);
	//texID=read_png_file(aFileName, alpha_tex, aNewTexturePacket->m_cWidth, aNewTexturePacket->m_cHeight, aNewTexturePacket->m_cBpp);
	char metaDataFile[512];
	sprintf(metaDataFile, "%s/%s", m_szMetaDataFolder, aFileName);
	texID=read_texture2D_from_metafile(metaDataFile, alpha_tex, aNewTexturePacket->m_cWidth, aNewTexturePacket->m_cHeight, aNewTexturePacket->m_cBpp);

    aNewTexturePacket->bAlphaTex=alpha_tex;
    glDisable(GL_TEXTURE_2D);

    if(texID>0)
    {
        m_iTotalTextureMemory+=(unsigned int)(aNewTexturePacket->m_cWidth*aNewTexturePacket->m_cHeight*aNewTexturePacket->m_cBpp);
    }

	if(aFileName)
	{
		aNewTexturePacket->iTextureName = new char[strlen(aFileName)+1];
		strcpy(aNewTexturePacket->iTextureName, aFileName);
		aNewTexturePacket->iTextureName[strlen(aFileName)]	= '\0';
	}

	aNewTexturePacket->bAlphaTex	= alpha_tex;
	aNewTexturePacket->iTextureID	= texID;

	iTexturePacket.push_back(aNewTexturePacket);
	
	return aNewTexturePacket;
}

void CTextureManager::ReLoad()
{
//    m_iTotalTextureMemory=0;
//    for(int i=0;i<iTexturePacket.size();i++)
//	{
//		stTexturePacket* packet=iTexturePacket[i];
//		//packet->releaseTextureData();
//        
//        
//        unsigned int texID=0;
//        bool alpha_tex=false;
//        
//		//texture loading utility
//		glEnable(GL_TEXTURE_2D);
//		texID=objC_util::loadTexture2(packet->iTextureName, alpha_tex, packet->m_cWidth, packet->m_cHeight, packet->m_cBpp);
//        
//		packet->bAlphaTex=alpha_tex;
//        packet->iTextureID	= texID;
//
//        if(texID<=0)
//        {            
//#if defined (LOG_DEBUG_ENGINE)
//            DEBUG_PRINT("texID is zero !!!!!");
//#endif
//        }
//        else
//        {
//            m_iTotalTextureMemory+=(unsigned int)(packet->m_cWidth*packet->m_cHeight*packet->m_cBpp);
//        }
//        
//		glDisable(GL_TEXTURE_2D);
//
//	}
}

void CTextureManager::Reset()
{
	for(int i=0;i<iTexturePacket.size();i++)
	{
		stTexturePacket* packet=iTexturePacket[i];
		GX_DELETE(packet);
	}
	iTexturePacket.clear();
    m_iTotalTextureMemory=0;
}

unsigned int read_texture2D_from_metafile(const char* file_name, bool& bAlpha, unsigned int& cx, unsigned int& cy, unsigned int& bpp)
{
	stMetaHeader metaHeader;
	gxFile file_meta;
	if(file_meta.OpenFile(file_name))
	{
		file_meta.ReadBuffer((unsigned char*)&metaHeader, sizeof(metaHeader));

		//read the text header
		file_meta.Read(cx);
		file_meta.Read(cy);
		int type=0;
		file_meta.Read(type);
		GLenum texType=GL_RGB;

		if(type==eTexture2D_888)
		{
			bAlpha=false;
			bpp=3;
			texType=GL_RGB;
		}
		else if(type==eTexture2D_8888)
		{
			bAlpha=true;
			bpp=4;
			texType=GL_RGBA;
		}
		else
		{
			file_meta.CloseFile();
			return 0;
		}

		unsigned int texID=0;
		glEnable(GL_TEXTURE_2D);
		glGenTextures(1, &texID);
		glBindTexture(GL_TEXTURE_2D, texID);
        
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

		unsigned char* image_data = new unsigned char[cx*cy*bpp];
		file_meta.ReadBuffer(image_data, cx*cy*bpp);
		file_meta.CloseFile();

		glTexImage2D(GL_TEXTURE_2D, 0, texType, cx, cy, 0, texType, GL_UNSIGNED_BYTE, image_data);

		GX_DELETE(image_data);

		return texID;
	}

	return 0;
}