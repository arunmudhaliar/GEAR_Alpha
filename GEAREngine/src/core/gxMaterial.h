#pragma once

#include "vector4.h"
#include "TextureManager.h"
#include <algorithm>
#include "gxSurfaceShader.h"

struct stMaterialPass
{
	stMaterialPass()
	{
		pass=NULL;
	}
	~stMaterialPass()
	{
		vUsedSubMap.clear();
	}
	stPass* pass;
	std::vector<gxSubMap*> vUsedSubMap;
};

class DECLSPEC gxMaterial : public GEARAsset, public Ref
{
public:
    gxMaterial();
    ~gxMaterial();
    static gxMaterial* create();

	bool isTwoSided()                   {	return twoSided;	}

	const vector4f& getAmbientClr()		{	return ambientColor;	}
	const vector4f& getDiffuseClr()		{	return diffuseColor;	}
	const vector4f& getSpecularClr()	{	return specularColor;	}

	void setAmbientClr(vector4f clr)	{	ambientColor=clr;		}
	void setDiffuseClr(vector4f clr)	{	diffuseColor=clr;		}
	void setSpecularClr(vector4f clr)	{	specularColor=clr;	}

	float getShininess()				{	return shininess;	}
	void setShininess(float value)		{	shininess=value;	}

    void setMaterialName(const std::string& name)	{	materialName = name;	}
    const std::string& getMaterialName()            {	return materialName;    }

	bool appendDependency(int crc);

	void write(gxFile& file);
	void read(gxFile& file);

	static gxMaterial* createNewMaterial();

	void setMainShaderName(const char* name)	{	mainShaderName.assign(name);	}
	const char* getMainshaderName()				{	return mainShaderName.c_str();	}

	void setSurfaceShader(gxSurfaceShader* surfaceShader);
	gxSurfaceShader* getSurfaceShader()						{	return surfaceShader;	}

	void appendSubMap(gxSubMap* map);
	gxSubMap* getSubMap(int index);
	std::vector<gxSubMap*>* getSubMapList()             {	return &subMapList;         }
	std::vector<stMaterialPass*>* getShaderPassList()	{	return &materialPassList;	}

	void appendTextureNamesFromFBX(const char* filename);
	std::vector<std::string>* getListOfTextureNamesFromFBX()	{  return &textureNameFromFBXList;	}

private:
	vector4f ambientColor;
	vector4f diffuseColor;
	vector4f specularColor;
	float alpha;
	float shininess;
	bool twoSided;
    std::string materialName;
	std::vector<int> dependencyCRCList;
	std::string mainShaderName;
	gxSurfaceShader* surfaceShader;	//must not delete this pointer
	std::vector<gxSubMap*> subMapList;
	std::vector<stMaterialPass*> materialPassList;

	//texture names read from FBX
	std::vector<std::string> textureNameFromFBXList;
	//
};