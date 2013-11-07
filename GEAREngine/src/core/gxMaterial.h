#ifndef GXMATERIAL_H
#define GXMATERIAL_H

#include "vector4.h"
#include "TextureManager.h"
#include <algorithm>
#include "../hwShader/gxHWShader.h"
#include "gxSurfaceShader.h"

class DllExport gxMaterial : public gxSurfaceShader
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

	gxHWShader* m_pShader;
};

#endif
