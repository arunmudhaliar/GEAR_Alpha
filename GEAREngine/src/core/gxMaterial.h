#ifndef GXMATERIAL_H
#define GXMATERIAL_H

#include "gxSubMaterialMap.h"
#include "vector4.h"
#include "TextureManager.h"
#include "../util/gxFile.h"
#include "../util/gxUtil.h"
#include <algorithm>

//#define MAX_SUBMATERIALMAP	8

class gxMaterial// : public objectBase
{
public:
	gxMaterial()
	{
		//memset(m_pszSubMap, 0, sizeof(m_pszSubMap));
		m_pTexture=NULL;
		memset(m_szTextureName, 0, sizeof(m_szTextureName));

		m_cDiffuse.set(0.5f, 0.5f, 0.5f, 1.0f);
		m_iFileCRC=0;
	}

	~gxMaterial()
	{
		//for(int x=0;x<gxSubMaterialMap::EMMAP_MAX;x++)
		//{
		//	gxSubMaterialMap* subMap=m_pszSubMap[x];
		//	GX_DELETE(subMap);
		//}

		GX_DELETE(m_pTexture);
	}

	//bool isAlphaOrBlended()
	//{
	//	bool bAlphaOrBlended=false;
	//
	//	//chk on the shader array
	//	for(int x=0;x<gxSubMaterialMap::EMMAP_MAX;x++)
	//	{
	//		gxSubMaterialMap* subMatMap=m_pszSubMap[x];

	//		if(!subMatMap) continue;
	//	
	//		if(subMatMap->getTexture()->getTextureType()==gxTexture::TEX_ALPHA)
	//		{
	//			bAlphaOrBlended=true;
	//			break;
	//		}
	//	}
	//
	//	//chk if transparency value is other than 1.0f
	//	if(m_fAlpha<1.0f)
	//		bAlphaOrBlended=true;
	//	
	//	return bAlphaOrBlended;
	//}

	bool isTwoSided()	{	return m_bTwoSided;	}

	const vector4f& getAmbientClr()		{	return m_cAmbient;	}
	const vector4f& getDiffuseClr()		{	return m_cDiffuse;	}
	const vector4f& getSpecularClr()	{	return m_cSpecular;	}

	void setAmbientClr(vector4f& clr)		{	m_cAmbient=clr;		}
	void setDiffuseClr(vector4f& clr)		{	m_cDiffuse=clr;		}
	void setSpecularClr(vector4f& clr)		{	m_cSpecular=clr;	}

	float getShininess()				{	return m_fShininess;	}

	//gxSubMaterialMap* getSubMaterialMap(int index)	{	return m_pszSubMap[index];	}

	void setMaterialName(const char* name)	{	strcpy(m_szMaterialName, name);	}
	const char* getMaterialName()			{	return m_szMaterialName;		}

	gxTexture* getTexture()					{	return m_pTexture;	}

	gxTexture* loadTextureFromDirectory(CTextureManager& textureManager, const char* directory)
	{
		if(strlen(m_szTextureName)==0)
			return NULL;
		char metaInfoFileName[256];
		sprintf(metaInfoFileName, "%s//%s.meta",directory, m_szTextureName);

		gxFile metaInfoFile;
		if(metaInfoFile.OpenFile(metaInfoFileName))
		{
			int crc=0;
			metaInfoFile.Read(crc);
			metaInfoFile.CloseFile();

			char metaDataFileName[256];
			sprintf(metaDataFileName, "%x", crc);

			stTexturePacket* texturePack=textureManager.LoadTexture(metaDataFileName);

			GX_DELETE(m_pTexture);
			m_pTexture = new gxTexture();
			m_pTexture->setTexture(texturePack);
			if(texturePack->bAlphaTex)
			{
				m_pTexture->setTextureType(gxTexture::TEX_ALPHA);
			}
			else
			{
				m_pTexture->setTextureType(gxTexture::TEX_NORMAL);
			}

			return m_pTexture;
		}

		return NULL;
	}

	gxTexture* loadTextureFromFile(CTextureManager& textureManager, const char* filename)
	{
/*		if(strlen(m_szTextureName)==0)
			return NULL;*/
		char metaInfoFileName[256];
		const char* onlyFilename = gxUtil::getFileNameFromPath(filename);
		setTextureName(onlyFilename);

		sprintf(metaInfoFileName, "%s.meta", filename);

		gxFile metaInfoFile;
		if(metaInfoFile.OpenFile(metaInfoFileName))
		{
			int crc=0;
			metaInfoFile.Read(crc);
			metaInfoFile.CloseFile();

			char metaDataFileName[256];
			sprintf(metaDataFileName, "%x", crc);

			stTexturePacket* texturePack=textureManager.LoadTexture(metaDataFileName);
			if(texturePack)
			{
				GX_DELETE(m_pTexture);
				m_pTexture = new gxTexture();
				m_pTexture->setTexture(texturePack);
				if(texturePack->bAlphaTex)
				{
					m_pTexture->setTextureType(gxTexture::TEX_ALPHA);
				}
				else
				{
					m_pTexture->setTextureType(gxTexture::TEX_NORMAL);
				}
			}
			return m_pTexture;
		}

		return NULL;
	}
	void setTextureName(const char* textureName)
	{
		strcpy(m_szTextureName, textureName);
	}

	bool appendDependency(int crc)
	{
		if(std::find(m_vDependencyCRCList.begin(), m_vDependencyCRCList.end(), crc)==m_vDependencyCRCList.end())
		{
			m_vDependencyCRCList.push_back(crc);
			return true;
		}

		return false;
	}

	void write(gxFile& file)
	{
		file.Write(m_iFileCRC);
		file.WriteBuffer((unsigned char*)&m_cAmbient, sizeof(m_cAmbient));
		file.WriteBuffer((unsigned char*)&m_cDiffuse, sizeof(m_cDiffuse));
		file.WriteBuffer((unsigned char*)&m_cSpecular, sizeof(m_cSpecular));
		file.Write(m_fAlpha);
		file.Write(m_fShininess);
		file.Write(m_bTwoSided);
		file.Write(m_szMaterialName);
		file.Write(m_szTextureName);
		file.Write(m_vDependencyCRCList.size());
		for(int x=0;x<m_vDependencyCRCList.size();x++)
		{
			file.Write(m_vDependencyCRCList[x]);
		}
	}

	void read(gxFile& file)
	{
		file.Read(m_iFileCRC);
		file.ReadBuffer((unsigned char*)&m_cAmbient, sizeof(m_cAmbient));
		file.ReadBuffer((unsigned char*)&m_cDiffuse, sizeof(m_cDiffuse));
		file.ReadBuffer((unsigned char*)&m_cSpecular, sizeof(m_cSpecular));
		file.Read(m_fAlpha);
		file.Read(m_fShininess);
		file.Read(m_bTwoSided);	
		char* temp=file.ReadString();
		strcpy(m_szMaterialName, temp);
		GX_DELETE_ARY(temp);
		temp=file.ReadString();
		strcpy(m_szTextureName, temp);
		GX_DELETE_ARY(temp);
		int nDep=0;
		file.Read(nDep);
		for(int x=0;x<nDep;x++)
		{
			int crc=0;
			file.Read(crc);
			m_vDependencyCRCList.push_back(crc);
		}
	}

	void setFileCRC(int crc)	{	m_iFileCRC=crc;		}
	int getFileCRC()			{	return m_iFileCRC;	}

private:
	vector4f m_cAmbient;
	vector4f m_cDiffuse;
	vector4f m_cSpecular;
	float m_fAlpha;
	float m_fShininess;
	bool m_bTwoSided;
	//gxSubMaterialMap* m_pszSubMap[gxSubMaterialMap::EMMAP_MAX];
	gxTexture* m_pTexture;
	char m_szMaterialName[64];
	char m_szTextureName[256];
	std::vector<int> m_vDependencyCRCList;
	int m_iFileCRC;
};

#endif
