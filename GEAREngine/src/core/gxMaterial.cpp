#include "gxMaterial.h"

gxMaterial::gxMaterial():
	GEARAsset()
{
	m_cDiffuse.set(0.5f, 0.5f, 0.5f, 1.0f);
	m_cAmbient.set(0.1f, 0.1f, 0.1f, 1.0f);
	m_cSpecular.set(0.2f, 0.2f, 0.2f, 1.0f);
	m_pSurfaceShaderPtr=NULL;
	m_fAlpha=1.0f;
	m_fShininess=10.0f;
	m_bTwoSided=false;
}

gxMaterial::~gxMaterial()
{
	for(std::vector<gxSubMap*>::iterator it = m_vSubMap.begin(); it != m_vSubMap.end(); ++it)
	{
		gxSubMap* map = *it;
		GX_DELETE(map);
	}
	m_vSubMap.clear();

	//clear all the material passes
	for(std::vector<stMaterialPass*>::iterator it_pass = m_vMaterialPass.begin(); it_pass != m_vMaterialPass.end(); ++it_pass)
	{
		stMaterialPass* mpass=*it_pass;
		GX_DELETE(mpass);
	}
	m_vMaterialPass.clear();
	//

	m_vTextureNameFromFBXFile.clear();
}

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
	file.Write(m_iAssetFileCRC);
	file.WriteBuffer((unsigned char*)&m_cAmbient, sizeof(m_cAmbient));
	file.WriteBuffer((unsigned char*)&m_cDiffuse, sizeof(m_cDiffuse));
	file.WriteBuffer((unsigned char*)&m_cSpecular, sizeof(m_cSpecular));
	file.Write(m_fAlpha);
	file.Write(m_fShininess);
	file.Write(m_bTwoSided);
	file.Write(m_szMaterialName);
	file.Write((int)m_vSubMap.size());
	for(std::vector<gxSubMap*>::iterator it = m_vSubMap.begin(); it != m_vSubMap.end(); ++it)
	{
		gxSubMap* submap = *it;
		file.Write(submap->getTextureName());
		file.Write(submap->getTextureCRC());
	}

	file.Write(m_cMainShaderName.c_str());
	file.Write((int)m_vDependencyCRCList.size());
	for(int x=0;x<m_vDependencyCRCList.size();x++)
	{
		file.Write(m_vDependencyCRCList[x]);
	}

	file.Write((int)m_vTextureNameFromFBXFile.size());
	for(int x=0;x<m_vTextureNameFromFBXFile.size();x++)
	{
		file.Write(m_vTextureNameFromFBXFile[x].c_str());
	}
}

void gxMaterial::read(gxFile& file)
{
	file.Read(m_iAssetFileCRC);
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

		gxSubMap* submap = new gxSubMap();
		submap->setTextureName(temp);
		submap->setTextureCRC(texcrc);
		m_vSubMap.push_back(submap);
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

	int nFBXTextures=0;
	file.Read(nFBXTextures);
	for(int x=0;x<nFBXTextures;x++)
	{
		char* temp=file.ReadString();
		m_vTextureNameFromFBXFile.push_back(temp);
		GX_DELETE_ARY(temp);
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

void gxMaterial::setSurfaceShader(gxSurfaceShader* surfaceShader)
{
	if(m_pSurfaceShaderPtr==surfaceShader || surfaceShader==NULL) return;

	m_pSurfaceShaderPtr=surfaceShader;

	std::vector<stShaderProperty_Texture2D*>* propertylist=m_pSurfaceShaderPtr->getShaderPropertyList_Texture2D();
	while(m_vSubMap.size()<propertylist->size())
	{
		gxSubMap* submap = new gxSubMap();
		appendSubMap(submap);
	}

#if REMOVE_ADDITIONAL_MAPS_FROM_LIST
	//remove additional maps from list
	while(m_vSubMap.size()>propertylist->size())
	{
		gxSubMap* submap=m_vSubMap[m_vSubMap.size()-1];
		m_vSubMap.erase(std::remove(m_vSubMap.begin(), m_vSubMap.end(), submap), m_vSubMap.end());
		GX_DELETE(submap);
	}
	//
#endif

	int submap_cntr=0;
	for(std::vector<stShaderProperty_Texture2D*>::iterator it = propertylist->begin(); it != propertylist->end(); ++it, submap_cntr++)
	{
		stShaderProperty_Texture2D* tex2d = *it;
		gxSubMap* submap = m_vSubMap[submap_cntr];
		submap->setShaderTextureProperty(tex2d);
	}

	//clear all the material passes
	for(std::vector<stMaterialPass*>::iterator it_pass = m_vMaterialPass.begin(); it_pass != m_vMaterialPass.end(); ++it_pass)
	{
		stMaterialPass* mpass=*it_pass;
		GX_DELETE(mpass);
	}
	m_vMaterialPass.clear();
	//

	std::vector<stPass*>* passlist=m_pSurfaceShaderPtr->getShaderPassList();
	int cntr=0;
	for(std::vector<stPass*>::iterator it_pass = passlist->begin(); it_pass != passlist->end(); ++it_pass, cntr++)
	{
		stPass* currentPass = *it_pass;
		stMaterialPass* mpass = new stMaterialPass();
		mpass->pass=currentPass;
		
		//check if a tex coord is used in this pass or not
		for(std::vector<gxSubMap*>::iterator submap_it = m_vSubMap.begin(); submap_it != m_vSubMap.end(); ++submap_it)
		{
			gxSubMap* map = *submap_it;
			//check in vertex shader
			if((int)currentPass->vertex_buffer.find(map->getShaderTextureProperty()->texture_uv_in_name)>=0)
			{
				mpass->vUsedSubMap.push_back(map);
			}

			//check in fragment shader
			if((int)currentPass->fragment_buffer.find(map->getShaderTextureProperty()->texture_uv_in_name)>=0)
			{
				mpass->vUsedSubMap.push_back(map);
			}
		}

		m_vMaterialPass.push_back(mpass);
	}
}

void gxMaterial::appendSubMap(gxSubMap* map)
{
	m_vSubMap.push_back(map);
}

gxSubMap* gxMaterial::getSubMap(int index)
{
	if(index>=(int)m_vSubMap.size()) return NULL;

	return m_vSubMap[index];
}

void gxMaterial::appendTextureNamesFromFBX(const char* filename)
{
	m_vTextureNameFromFBXFile.push_back(filename);
}