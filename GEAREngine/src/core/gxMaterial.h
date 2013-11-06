#ifndef GXMATERIAL_H
#define GXMATERIAL_H

#include "gxSubMaterialMap.h"
#include "vector4.h"
#include "TextureManager.h"
#include "../util/gxFile.h"
#include "../util/gxUtil.h"
#include <algorithm>
#include "../hwShader/gxHWShader.h"

//#define MAX_SUBMATERIALMAP	8

class DllExport gxSubMap
{
public:
	gxSubMap();
	~gxSubMap();

	//enum ESUBMAP
	//{
	//	MAP_DIFFUSE,
	//	MAP_NORMAL,
	//	MAP_SPECULAR,
	//	MAP_MAX
	//};
	
	void setTextureName(const char* name)	{	GX_STRCPY(m_szTextureName, name);	}
	//void setTextureType(ESUBMAP type)		{	m_eType=type;		}
	void setTextureCRC(int crc)				{	m_iTextureCRC=crc;	}
	const char* getTextureName()			{	return m_szTextureName;	}
	//ESUBMAP getTextureType()				{	return m_eType;			}
	int getTextureCRC()						{	return m_iTextureCRC;	}
	gxTexture* load(CTextureManager& textureManager, const char* filename);
	gxTexture* getTexture()		{	return m_pTexture;	}
	gxTexture* loadTextureFromMeta(CTextureManager& textureManager, int crc);

private:

	char m_szTextureName[256];
	int m_iTextureCRC;
	gxTexture* m_pTexture;
	//ESUBMAP m_eType;
};

class DllExport gxMaterial// : public objectBase
{
public:
	gxMaterial();
	~gxMaterial();

	bool isTwoSided()	{	return m_bTwoSided;	}

	const vector4f& getAmbientClr()		{	return m_cAmbient;	}
	const vector4f& getDiffuseClr()		{	return m_cDiffuse;	}
	const vector4f& getSpecularClr()	{	return m_cSpecular;	}

	void setAmbientClr(vector4f& clr)		{	m_cAmbient=clr;		}
	void setDiffuseClr(vector4f& clr)		{	m_cDiffuse=clr;		}
	void setSpecularClr(vector4f& clr)		{	m_cSpecular=clr;	}

	float getShininess()				{	return m_fShininess;	}

	//gxSubMaterialMap* getSubMaterialMap(int index)	{	return m_pszSubMap[index];	}

	void setMaterialName(const char* name)	{	GX_STRCPY(m_szMaterialName, name);	}
	const char* getMaterialName()			{	return m_szMaterialName;		}

	//gxTexture* getTexture()					{	return m_pTexture;	}

#if 0
	gxTexture* loadTextureFromDirectory(CTextureManager& textureManager, const char* directory);
#endif

	gxTexture* loadTextureFromFile(CTextureManager& textureManager, const char* filename, int submap);	//-1 will add a map to the list

	void appendSubMap(gxSubMap* map);

	//void setTextureName(const char* textureName);
	bool appendDependency(int crc);

	void write(gxFile& file);
	void read(gxFile& file);

	void setFileCRC(int crc)	{	m_iFileCRC=crc;		}
	int getFileCRC()			{	return m_iFileCRC;	}
	//int getTextureReadCRC()		{	return m_iTextureFileCRC;	}

	static gxMaterial* createNewMaterial();

	//void setSubMap(gxSubMap* submap, gxSubMap::ESUBMAP eType);
	gxSubMap* getSubMap(int index);
	std::vector<gxSubMap*>* getSubMapList()	{	return &m_vSubMap;	}

private:
	vector4f m_cAmbient;
	vector4f m_cDiffuse;
	vector4f m_cSpecular;
	float m_fAlpha;
	float m_fShininess;
	bool m_bTwoSided;
	//gxSubMaterialMap* m_pszSubMap[gxSubMaterialMap::EMMAP_MAX];
	//gxTexture* m_pTexture;
	char m_szMaterialName[64];
	//char m_szTextureName[256];
	std::vector<int> m_vDependencyCRCList;
	int m_iFileCRC;
	//int m_iTextureFileCRC;

	gxHWShader* m_pShader;
	std::vector<gxSubMap*> m_vSubMap;//[gxSubMap::MAP_MAX];
};

#endif
