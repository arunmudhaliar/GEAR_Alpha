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

class DECLSPEC gxMaterial : public GEARAsset
{
public:
	gxMaterial();
	~gxMaterial();

	bool isTwoSided()	{	return m_bTwoSided;	}

	const vector4f& getAmbientClr()		{	return m_cAmbient;	}
	const vector4f& getDiffuseClr()		{	return m_cDiffuse;	}
	const vector4f& getSpecularClr()	{	return m_cSpecular;	}

	void setAmbientClr(vector4f clr)		{	m_cAmbient=clr;		}
	void setDiffuseClr(vector4f clr)		{	m_cDiffuse=clr;		}
	void setSpecularClr(vector4f clr)		{	m_cSpecular=clr;	}

	float getShininess()				{	return m_fShininess;	}
	void setShininess(float value)		{	m_fShininess=value;	}

	void setMaterialName(const char* name)	{	GX_STRCPY(m_szMaterialName, name);	}
	const char* getMaterialName()			{	return m_szMaterialName;		}

	bool appendDependency(int crc);

	void write(gxFile& file);
	void read(gxFile& file);

	static gxMaterial* createNewMaterial();

	void setMainShaderName(const char* name)	{	m_cMainShaderName.assign(name);		}
	const char* getMainshaderName()				{	return m_cMainShaderName.c_str();	}

	void setSurfaceShader(gxSurfaceShader* surfaceShader);
	gxSurfaceShader* getSurfaceShader()						{	return m_pSurfaceShaderPtr;			}

	void appendSubMap(gxSubMap* map);
	gxSubMap* getSubMap(int index);
	std::vector<gxSubMap*>* getSubMapList()	{	return &m_vSubMap;	}
	std::vector<stMaterialPass*>* getShaderPassList()	{	return &m_vMaterialPass;	}

	void appendTextureNamesFromFBX(const char* filename);
	std::vector<std::string>* getListOfTextureNamesFromFBX()	{  return &m_vTextureNameFromFBXFile;	}

private:
	vector4f m_cAmbient;
	vector4f m_cDiffuse;
	vector4f m_cSpecular;
	float m_fAlpha;
	float m_fShininess;
	bool m_bTwoSided;
	char m_szMaterialName[256];
	std::vector<int> m_vDependencyCRCList;
	std::string m_cMainShaderName;
	gxSurfaceShader* m_pSurfaceShaderPtr;	//must not delete this pointer
	std::vector<gxSubMap*> m_vSubMap;
	std::vector<stMaterialPass*> m_vMaterialPass;

	//texture names read from FBX
	std::vector<std::string> m_vTextureNameFromFBXFile;
	//
};