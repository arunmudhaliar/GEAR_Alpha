#ifndef GXSURFACESHADER_H
#define GXSURFACESHADER_H

#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <string>
#include <vector>
#include "vector4.h"
#include "../core/TextureManager.h"
#include "gxTexture.h"
#include "../util/gxUtil.h"
#include "../util/gxFile.h"

class DllExport gxSubMap
{
public:
	gxSubMap();
	~gxSubMap();

	void setTextureName(const char* name)	{	GX_STRCPY(m_szTextureName, name);	}
	void setTextureCRC(int crc)				{	m_iTextureCRC=crc;	}
	const char* getTextureName()			{	return m_szTextureName;	}
	int getTextureCRC()						{	return m_iTextureCRC;	}
	gxTexture* load(CTextureManager& textureManager, const char* filename);
	gxTexture* getTexture()		{	return m_pTexture;	}
	gxTexture* loadTextureFromMeta(CTextureManager& textureManager, int crc);

private:
	char m_szTextureName[256];
	int m_iTextureCRC;
	gxTexture* m_pTexture;
};

struct stShaderProperty_Color
{
	std::string name;
	vector4f color;
};

struct stShaderProperty_Texture2D
{
	std::string name;
	std::string texture_name;
};

struct stSubShader
{
	std::string buffer;
};

class DllExport gxSurfaceShader
{
private:
	bool findQuat(std::string::const_iterator& start, std::string::const_iterator& end, bool bStrict=true);
	bool findOpeningRoundBrace(std::string::const_iterator& start, std::string::const_iterator& end);
	bool findClosingRoundBrace(std::string::const_iterator& start, std::string::const_iterator& end);
	bool findOpeningCurlyBrace(std::string::const_iterator& start, std::string::const_iterator& end);
	bool findClosingCurlyBrace(std::string::const_iterator& start, std::string::const_iterator& end);
	bool findComma(std::string::const_iterator& start, std::string::const_iterator& end);
	bool findEqualTo(std::string::const_iterator& start, std::string::const_iterator& end);
	bool findAnyValidCharForName(std::string::const_iterator& start, std::string::const_iterator& end);
	bool findAnyValidAlphaNumeric(std::string::const_iterator& start, std::string::const_iterator& end);
	bool findAnyValidNumeric(std::string::const_iterator& start, std::string::const_iterator& end);
	bool parseKeyWord(std::string::const_iterator& start, std::string::const_iterator& end, std::string& name);
	bool parseArgInsideRoundBrace(std::string::const_iterator& start, std::string::const_iterator& end, std::vector<std::string>& args, int argCount);
	bool parseNameInsideQuats(std::string::const_iterator& start, std::string::const_iterator& end, std::string& name);
	bool parseColorProperty(std::string::const_iterator& start, std::string::const_iterator& end, int& depth);
	bool parseMainTexProperty(std::string::const_iterator& start, std::string::const_iterator& end, int& depth);
	bool parseProperties(std::string::const_iterator& start, std::string::const_iterator& end, int& depth);
	bool parseSubShader(std::string::const_iterator& start, std::string::const_iterator& end, int& depth);
	bool parseShader(std::string::const_iterator& start, std::string::const_iterator& end, int& depth);
	bool parse(std::string::const_iterator& start, std::string::const_iterator& end, int& depth);

protected:
	stSubShader m_cSubShader;
	std::vector<stShaderProperty_Texture2D*> m_vTex2D_Properties;
	std::vector<stShaderProperty_Color*> m_vColor_Properties;

	std::vector<gxSubMap*> m_vSubMap;

public:
	gxSurfaceShader();
	virtual ~gxSurfaceShader();
	bool loadSurfaceShader(const char* filename);

	gxTexture* loadTextureFromFile(CTextureManager& textureManager, const char* filename, int submap);	//-1 will add a map to the list

	void appendSubMap(gxSubMap* map);
	gxSubMap* getSubMap(int index);
	std::vector<gxSubMap*>* getSubMapList()	{	return &m_vSubMap;	}
};
#endif