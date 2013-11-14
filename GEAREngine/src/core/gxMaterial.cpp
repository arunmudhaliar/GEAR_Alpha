#include "gxMaterial.h"

gxMaterial::gxMaterial()
{
	m_cDiffuse.set(0.5f, 0.5f, 0.5f, 1.0f);
	m_iFileCRC=0;
}

gxMaterial::~gxMaterial()
{
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

#if 0
gxTexture* gxMaterial::loadTextureFromDirectory(CTextureManager& textureManager, const char* directory)
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
#endif

//void gxMaterial::setSubMap(gxSubMap* submap, gxSubMap::ESUBMAP eType)
//{
//	GX_DELETE(m_pszSubMap[eType]);
//	m_pszSubMap[eType]=submap;
//}


bool gxMaterial::appendDependency(int crc)
{
	if(std::find(m_vDependencyCRCList.begin(), m_vDependencyCRCList.end(), crc)==m_vDependencyCRCList.end())
	{
		m_vDependencyCRCList.push_back(crc);
		return true;
	}

	return false;
}

void gxMaterial::write(gxFile& file)
{
	file.Write(m_iFileCRC);
	file.WriteBuffer((unsigned char*)&m_cAmbient, sizeof(m_cAmbient));
	file.WriteBuffer((unsigned char*)&m_cDiffuse, sizeof(m_cDiffuse));
	file.WriteBuffer((unsigned char*)&m_cSpecular, sizeof(m_cSpecular));
	file.Write(m_fAlpha);
	file.Write(m_fShininess);
	file.Write(m_bTwoSided);
	file.Write(m_szMaterialName);
	file.Write((int)m_vTextureMap.size());
	for(std::vector<stTextureMap*>::iterator it = m_vTextureMap.begin(); it != m_vTextureMap.end(); ++it)
	{
		stTextureMap* texmap = *it;
		file.Write(texmap->texturename.c_str());
		file.Write(texmap->crc);
	}

	file.Write(m_cMainShaderName.c_str());
	file.Write(m_vDependencyCRCList.size());
	for(int x=0;x<m_vDependencyCRCList.size();x++)
	{
		file.Write(m_vDependencyCRCList[x]);
	}
}

void gxMaterial::read(gxFile& file)
{
	file.Read(m_iFileCRC);
	file.ReadBuffer((unsigned char*)&m_cAmbient, sizeof(m_cAmbient));
	file.ReadBuffer((unsigned char*)&m_cDiffuse, sizeof(m_cDiffuse));
	file.ReadBuffer((unsigned char*)&m_cSpecular, sizeof(m_cSpecular));
	file.Read(m_fAlpha);
	file.Read(m_fShininess);
	file.Read(m_bTwoSided);	
	char* temp=file.ReadString();
	GX_STRCPY(m_szMaterialName, temp);
	GX_DELETE_ARY(temp);

	int nsubmap=0;
	file.Read(nsubmap);
	for(int x=0;x<nsubmap;x++)
	{
		char* temp=file.ReadString();
		int texcrc;
		file.Read(texcrc);

		stTextureMap* texmap = new stTextureMap();
		texmap->texturename.assign(temp);
		texmap->crc=texcrc;
		m_vTextureMap.push_back(texmap);
		GX_DELETE_ARY(temp);
	}

	temp=file.ReadString();
	setMainShaderName(temp);
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

gxMaterial* gxMaterial::createNewMaterial()
{
	gxMaterial* material = new gxMaterial();
	material->setMaterialName("New Material");

	material->setDiffuseClr(vector4f(0.7f, 0.7f, 0.7f, 1.0f));
	material->setAmbientClr(vector4f(0.7f, 0.7f, 0.7f, 1.0f));
	material->setSpecularClr(vector4f(0.2f, 0.2f, 0.2f, 1.0f));

	return material;
}