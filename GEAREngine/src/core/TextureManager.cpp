#include "TextureManager.h"
#include "gxMetaStructures.h"
#include "../util/gxFile.h"

#if defined(WIN32)
#include "../util/pngLoader.h"
#endif

unsigned int read_texture2D_from_metafile(const char* file_name, bool& bAlpha, unsigned int& cx, unsigned int& cy, unsigned int& bpp);

CTextureManager::CTextureManager()
{
    totalTextureMemory=0;
	texturePacket1x1=NULL;
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
        totalTextureMemory+=(unsigned int)(whiteTexPack->m_cWidth*whiteTexPack->m_cHeight*whiteTexPack->m_cBpp);
    }

	whiteTexPack->iTextureName = new char[strlen("gearWhiteTex1x1")+1];
	strcpy(whiteTexPack->iTextureName, "gearWhiteTex1x1");
	whiteTexPack->iTextureName[strlen("gearWhiteTex1x1")]	= '\0';

	texturePacket1x1 = whiteTexPack;
	texturePacketList.push_back(whiteTexPack);
}

stTexturePacket* CTextureManager::LoadTexture(const char* aFileName)
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
		if(i!=(int)texturePacketList.size())
			return texturePacketList[i];	//if texture already exist in list
	}//only diffuse


	stTexturePacket*	aNewTexturePacket	= new stTexturePacket();
	
	unsigned int texID=0;
	bool alpha_tex=false;

    //texture loading utility
    glEnable(GL_TEXTURE_2D);
	//texID=read_png_file(aFileName, alpha_tex, aNewTexturePacket->m_cWidth, aNewTexturePacket->m_cHeight, aNewTexturePacket->m_cBpp);
	char metaDataFile[512];
	sprintf(metaDataFile, "%s/%s", metaDataFolder, aFileName);
	texID=read_texture2D_from_metafile(metaDataFile, alpha_tex, aNewTexturePacket->m_cWidth, aNewTexturePacket->m_cHeight, aNewTexturePacket->m_cBpp);

    aNewTexturePacket->bAlphaTex=alpha_tex;
    glDisable(GL_TEXTURE_2D);

	if(texID==0)
	{
		DEBUG_PRINT("%s not found", metaDataFile);
		GX_DELETE(aNewTexturePacket);
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

	aNewTexturePacket->bAlphaTex	= alpha_tex;
	aNewTexturePacket->iTextureID	= texID;

	texturePacketList.push_back(aNewTexturePacket);
	
	return aNewTexturePacket;
}

void CTextureManager::ReLoad()
{
//    totalTextureMemory=0;
//    for(int i=0;i<texturePacketList.size();i++)
//	{
//		stTexturePacket* packet=texturePacketList[i];
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
//            totalTextureMemory+=(unsigned int)(packet->m_cWidth*packet->m_cHeight*packet->m_cBpp);
//        }
//        
//		glDisable(GL_TEXTURE_2D);
//
//	}
}

void CTextureManager::Reset()
{
	for(int i=0;i<texturePacketList.size();i++)
	{
		stTexturePacket* packet=texturePacketList[i];
		GX_DELETE(packet);
	}
	texturePacketList.clear();
    totalTextureMemory=0;
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
		GLint internalformat=GL_RGB;
		/*
	eTexture2D_Unknown,
	eTexture2D_888,
	eTexture2D_8888,
	eTexture2D_565,
	eTexture2D_5551,
	eTexture2D_BGR888,
	eTexture2D_BGRA8888,
		*/

		switch(type)
		{
		case eTexture2D_RGB888:
			{
				bAlpha=false;
				bpp=3;
				texType=GL_RGB;
				internalformat=GL_RGB;
			}
			break;
		case eTexture2D_RGBA8888:
			{
				bAlpha=true;
				bpp=4;
				texType=GL_RGBA;
				internalformat=GL_RGBA;
			}
			break;
#if !defined(__APPLE__)	//TODO: need to fix this for android
		case eTexture2D_RGB565:
			{
				bAlpha=false;
				bpp=2;
				texType=GL_RGB565;
				internalformat=GL_RGB;
			}
			break;
#if !defined(ANDROID)	//TODO: need to fix this for android
		case eTexture2D_RGBA5551:
			{
#ifdef _WIN32
				file_meta.CloseFile();
				return 0;
#else
				bAlpha=true;
				bpp=2;
				texType=GL_RGB5551;
				internalformat=GL_RGBA;
#endif
			}
			break;
		case eTexture2D_BGR888:
			{
				bAlpha=false;
				bpp=3;
				texType=GL_BGR;
				internalformat=GL_RGB;
			}
			break;
		case eTexture2D_BGRA8888:
			{
				bAlpha=true;
				bpp=4;
				texType=GL_BGRA;
				internalformat=GL_RGBA;
			}
			break;
#endif
#endif
		case eTexture2D_Unknown:
			{
				file_meta.CloseFile();
				return 0;
			}
			break;
		default:
			{
				file_meta.CloseFile();
				return 0;
			}
			break;
		}

		unsigned int texID=0;
		glEnable(GL_TEXTURE_2D);
		glGenTextures(1, &texID);
		glBindTexture(GL_TEXTURE_2D, texID);
        
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);	//MIPMAP

		unsigned char* image_data = new unsigned char[cx*cy*bpp];
		file_meta.ReadBuffer(image_data, cx*cy*bpp);
		file_meta.CloseFile();

		glTexImage2D(GL_TEXTURE_2D, 0, internalformat, cx, cy, 0, texType, GL_UNSIGNED_BYTE, image_data);

		glGenerateMipmap(GL_TEXTURE_2D);  //Generate mipmaps now!!!

		GX_DELETE(image_data);

		return texID;
	}

	return 0;
}