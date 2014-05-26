#ifndef GXMATERIAL_H
#define GXMATERIAL_H

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

struct stFog
{
	stFog()
	{
		fog_density=0.0f;
		fog_start=200.0f;
		fog_end=1000.0f;
		calculateScale();
		fog_color.set(1.0f, 1.0f, 1.0f, 1.0f);
	}

	void setFogDensity(float density)	{	fog_density=density; }
	void setFogStart(float start)		{	fog_start=start; }
	void setFogEnd(float end)			{	fog_end=end; }
	void calculateScale()				{	fog_scale=1.0/(fog_end-fog_start);	}
	void setFogColor(vector4f clr)		{	fog_color=clr;	}

    float fog_density;
	float fog_start;
	float fog_end;
	float fog_scale;
	vector4f fog_color;
};

class DECLSPEC gxMaterial //: public gxSurfaceShader
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

	void setMaterialName(const char* name)	{	GX_STRCPY(m_szMaterialName, name);	}
	const char* getMaterialName()			{	return m_szMaterialName;		}

#if 0
	gxTexture* loadTextureFromDirectory(CTextureManager& textureManager, const char* directory);
#endif
	bool appendDependency(int crc);

	void write(gxFile& file);
	void read(gxFile& file);

	void setFileCRC(int crc)	{	m_iFileCRC=crc;		}
	int getFileCRC()			{	return m_iFileCRC;	}

	static gxMaterial* createNewMaterial();

	void setMainShaderName(const char* name)	{	m_cMainShaderName.assign(name);		}
	const char* getMainshaderName()				{	return m_cMainShaderName.c_str();	}

	void setSurfaceShader(gxSurfaceShader* surfaceShader);
	gxSurfaceShader* getSurfaceShader()						{	return m_pSurfaceShaderPtr;			}

	gxTexture* loadTextureFromFile(CTextureManager& textureManager, const char* filename, int submap);	//-1 will add a map to the list
	void appendSubMap(gxSubMap* map);
	gxSubMap* getSubMap(int index);
	std::vector<gxSubMap*>* getSubMapList()	{	return &m_vSubMap;	}
	std::vector<stMaterialPass*>* getShaderPassList()	{	return &m_vMaterialPass;	}

	stFog* getFog()	{	return &m_cFog; }

private:
	vector4f m_cAmbient;
	vector4f m_cDiffuse;
	vector4f m_cSpecular;
	float m_fAlpha;
	float m_fShininess;
	bool m_bTwoSided;
	char m_szMaterialName[64];
	std::vector<int> m_vDependencyCRCList;
	int m_iFileCRC;
	std::string m_cMainShaderName;
	gxSurfaceShader* m_pSurfaceShaderPtr;	//must not delete this pointer
	std::vector<gxSubMap*> m_vSubMap;
	std::vector<stMaterialPass*> m_vMaterialPass;
	stFog m_cFog;
};

#endif
