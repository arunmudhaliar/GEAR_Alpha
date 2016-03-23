#include "gxMaterial.h"

gxMaterial* gxMaterial::create()
{
    auto newObject = new gxMaterial();
    if(newObject)
    {
        newObject->autoRelease();
        REF_RETAIN(newObject);
        
        return newObject;
    }
    
    return nullptr;
}

gxMaterial::gxMaterial():
	GEARAsset(),
    Ref()
{
	diffuseColor.set(0.5f, 0.5f, 0.5f, 1.0f);
	ambientColor.set(0.1f, 0.1f, 0.1f, 1.0f);
	specularColor.set(0.2f, 0.2f, 0.2f, 1.0f);
	surfaceShader=NULL;
	alpha=1.0f;
	shininess=10.0f;
	twoSided=false;
}

gxMaterial::~gxMaterial()
{
	for(std::vector<gxSubMap*>::iterator it = subMapList.begin(); it != subMapList.end(); ++it)
	{
		gxSubMap* map = *it;
		GX_DELETE(map);
	}
	subMapList.clear();

	//clear all the material passes
	for(std::vector<stMaterialPass*>::iterator it_pass = materialPassList.begin(); it_pass != materialPassList.end(); ++it_pass)
	{
		stMaterialPass* mpass=*it_pass;
		GX_DELETE(mpass);
	}
	materialPassList.clear();
	//

	textureNameFromFBXList.clear();
}

bool gxMaterial::appendDependency(int crc)
{
	if(std::find(dependencyCRCList.begin(), dependencyCRCList.end(), crc)==dependencyCRCList.end())
	{
		dependencyCRCList.push_back(crc);
		return true;
	}

	return false;
}

void gxMaterial::write(gxFile& file)
{
	file.Write(assetFileCRC);
	file.WriteBuffer((unsigned char*)&ambientColor, sizeof(ambientColor));
	file.WriteBuffer((unsigned char*)&diffuseColor, sizeof(diffuseColor));
	file.WriteBuffer((unsigned char*)&specularColor, sizeof(specularColor));
	file.Write(alpha);
	file.Write(shininess);
	file.Write(twoSided);
	file.Write(materialName.c_str());
	file.Write((int)subMapList.size());
	for(std::vector<gxSubMap*>::iterator it = subMapList.begin(); it != subMapList.end(); ++it)
	{
		gxSubMap* submap = *it;
		file.Write(submap->getTextureName());
		file.Write(submap->getTextureCRC());
	}

	file.Write(mainShaderName.c_str());
	file.Write((int)dependencyCRCList.size());
	for(int x=0;x<dependencyCRCList.size();x++)
	{
		file.Write(dependencyCRCList[x]);
	}

	file.Write((int)textureNameFromFBXList.size());
	for(int x=0;x<textureNameFromFBXList.size();x++)
	{
		file.Write(textureNameFromFBXList[x].c_str());
	}
}

void gxMaterial::read(gxFile& file)
{
	file.Read(assetFileCRC);
	file.ReadBuffer((unsigned char*)&ambientColor, sizeof(ambientColor));
	file.ReadBuffer((unsigned char*)&diffuseColor, sizeof(diffuseColor));
	file.ReadBuffer((unsigned char*)&specularColor, sizeof(specularColor));

	file.Read(alpha);
	file.Read(shininess);
	file.Read(twoSided);	

	char* temp=file.ReadString();
    materialName.assign(temp, strlen(temp));
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
		subMapList.push_back(submap);
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
		dependencyCRCList.push_back(crc);
	}

	int nFBXTextures=0;
	file.Read(nFBXTextures);
	for(int x=0;x<nFBXTextures;x++)
	{
		char* temp=file.ReadString();
		textureNameFromFBXList.push_back(temp);
		GX_DELETE_ARY(temp);
	}
}

gxMaterial* gxMaterial::createNewMaterial()
{
    gxMaterial* material = gxMaterial::create();
	material->setMaterialName("New Material");

	material->setDiffuseClr(vector4f(0.7f, 0.7f, 0.7f, 1.0f));
	material->setAmbientClr(vector4f(0.7f, 0.7f, 0.7f, 1.0f));
	material->setSpecularClr(vector4f(0.2f, 0.2f, 0.2f, 1.0f));

	return material;
}

void gxMaterial::setSurfaceShader(gxSurfaceShader* surfaceShader)
{
	if(this->surfaceShader==surfaceShader || surfaceShader==NULL) return;

	this->surfaceShader=surfaceShader;

	std::vector<stShaderProperty_Texture2D*>* propertylist=this->surfaceShader->getShaderPropertyList_Texture2D();
	while(subMapList.size()<propertylist->size())
	{
		gxSubMap* submap = new gxSubMap();
		appendSubMap(submap);
	}

#if REMOVE_ADDITIONAL_MAPS_FROM_LIST
	//remove additional maps from list
	while(subMapList.size()>propertylist->size())
	{
		gxSubMap* submap=subMapList[subMapList.size()-1];
		subMapList.erase(std::remove(subMapList.begin(), subMapList.end(), submap), subMapList.end());
		GX_DELETE(submap);
	}
	//
#endif

	int submap_cntr=0;
	for(std::vector<stShaderProperty_Texture2D*>::iterator it = propertylist->begin(); it != propertylist->end(); ++it, submap_cntr++)
	{
		stShaderProperty_Texture2D* tex2d = *it;
		gxSubMap* submap = subMapList[submap_cntr];
		submap->setShaderTextureProperty(tex2d);
	}

	//clear all the material passes
	for(std::vector<stMaterialPass*>::iterator it_pass = materialPassList.begin(); it_pass != materialPassList.end(); ++it_pass)
	{
		stMaterialPass* mpass=*it_pass;
		GX_DELETE(mpass);
	}
	materialPassList.clear();
	//

	std::vector<stPass*>* passlist=this->surfaceShader->getShaderPassList();
	int cntr=0;
	for(std::vector<stPass*>::iterator it_pass = passlist->begin(); it_pass != passlist->end(); ++it_pass, cntr++)
	{
		stPass* currentPass = *it_pass;
		stMaterialPass* mpass = new stMaterialPass();
		mpass->pass=currentPass;
		
		//check if a tex coord is used in this pass or not
		for(std::vector<gxSubMap*>::iterator submap_it = subMapList.begin(); submap_it != subMapList.end(); ++submap_it)
		{
			gxSubMap* map = *submap_it;
			//check in vertex shader
			if((int)currentPass->vertexGLSLSourceBuffer.find(map->getShaderTextureProperty()->texture_uv_in_name)>=0)
			{
				mpass->vUsedSubMap.push_back(map);
			}

			//check in fragment shader
			if((int)currentPass->fragmentGLSLSourceBuffer.find(map->getShaderTextureProperty()->texture_uv_in_name)>=0)
			{
				mpass->vUsedSubMap.push_back(map);
			}
		}

		materialPassList.push_back(mpass);
	}
}

void gxMaterial::appendSubMap(gxSubMap* map)
{
	subMapList.push_back(map);
}

gxSubMap* gxMaterial::getSubMap(int index)
{
	if(index>=(int)subMapList.size()) return NULL;

	return subMapList[index];
}

void gxMaterial::appendTextureNamesFromFBX(const char* filename)
{
	textureNameFromFBXList.push_back(filename);
}