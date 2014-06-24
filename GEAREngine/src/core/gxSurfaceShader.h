#ifndef GXSURFACESHADER_H
#define GXSURFACESHADER_H

#include <stdio.h>
//#include <conio.h>
#include <iostream>
#include <string>
#include <vector>
#include "vector4.h"
#include "../core/TextureManager.h"
#include "gxTexture.h"
#include "../util/gxUtil.h"
#include "../util/gxFile.h"
#include "../hwShader/gxHWShader.h"

struct stTextureMap
{
	stTextureMap()
	{
		crc=0;
	}
	std::string texturename;
	int crc;
};


//////////Surface Shader Properties//////////
struct stShaderProperty_Vector
{
	~stShaderProperty_Vector()
	{
		name.clear();
	}
	std::string name;
	vector4f vector;
};

struct stShaderProperty_Range
{
	stShaderProperty_Range()
	{
		range_max=range_min=range_value=0.0f;
	}

	~stShaderProperty_Range()
	{
		name.clear();
	}
	std::string name;
	float range_max;
	float range_min;
	float range_value;
};

struct stShaderProperty_Color
{
	~stShaderProperty_Color()
	{
		name.clear();
	}
	std::string name;
	vector4f color;
};

struct stShaderProperty_Texture2D
{
	stShaderProperty_Texture2D()
	{
	}

	~stShaderProperty_Texture2D()
	{
		name.clear();
		texture_name.clear();
		texture_uv_in_name.clear();
		texture_uv_out_name.clear();
		texture_sampler2d_name.clear();
	}
	std::string name;
	std::string texture_name;
	std::string texture_uv_in_name;
	std::string texture_uv_out_name;
	std::string texture_sampler2d_name;
};
//////////~Surface Shader Properties//////////

class DECLSPEC gxSubMap
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

	void setShaderTextureProperty(stShaderProperty_Texture2D* property_tex2D)	{	m_pShaderPropertyVar = property_tex2D;	}
	stShaderProperty_Texture2D* getShaderTextureProperty()						{	return m_pShaderPropertyVar;			}

private:
	char m_szTextureName[256];
	int m_iTextureCRC;
	gxTexture* m_pTexture;
	stShaderProperty_Texture2D* m_pShaderPropertyVar;
};

struct stPass
{
	stPass()
	{
		GEAR_M=false;
		GEAR_M_INVERSE=false;
		GEAR_MV=false;
		GEAR_MVP=false;
		GEAR_NORMAL_MATRIX=false;
		Light=false;
		Material=false;
		vIN_Position=false;
		vIN_Normal=false;
		vIN_Color=false;
		Tangent=false;
		GEAR_Time=false;
		GEAR_ScreenParams=false;
		cull_face=GL_BACK;	//default value
	}

	~stPass()
	{
		vertex_buffer.clear();
		fragment_buffer.clear();
		vIncludeModule.clear();
	}

	bool GEAR_M;
	bool GEAR_M_INVERSE;
	bool GEAR_MV;
	bool GEAR_MVP;
	bool GEAR_NORMAL_MATRIX;
	bool Light;
	bool Material;
	bool vIN_Position;
	bool vIN_Normal;
	bool vIN_Color;
	bool Tangent;
	bool GEAR_Time;
	bool GEAR_ScreenParams;
	unsigned int cull_face;

	std::string vertex_buffer;
	std::string fragment_buffer;
	std::vector<std::string> vIncludeModule;
};

struct stSubShader
{
	~stSubShader()
	{
		for(std::vector<stPass*>::iterator it = m_vPass.begin(); it != m_vPass.end(); ++it)
		{
			stPass* pass = *it;
			GX_DELETE(pass);
		}
		m_vPass.clear();
	}

	std::vector<stPass*> m_vPass;
};
class DECLSPEC gxSurfaceShader
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
	bool findAnyValidFloat(std::string::const_iterator& start, std::string::const_iterator& end);
	bool parseKeyWord(std::string::const_iterator& start, std::string::const_iterator& end, std::string& name);
	bool parseFloat(std::string::const_iterator& start, std::string::const_iterator& end, float& floatval);
	bool parseArgInsideRoundBrace(std::string::const_iterator& start, std::string::const_iterator& end, std::vector<std::string>& args, int argCount);
	bool parseAlphaNumericArgInsideRoundBrace(std::string::const_iterator& start, std::string::const_iterator& end, std::vector<std::string>& args, int argCount);
	bool parseNameInsideQuats(std::string::const_iterator& start, std::string::const_iterator& end, std::string& name);
	bool parseEachProperty(std::string::const_iterator& start, std::string::const_iterator& end, const char* _nameofproperty, int& depth);
	bool parseProperties(std::string::const_iterator& start, std::string::const_iterator& end, int& depth);
	bool parseSubShader(std::string::const_iterator& start, std::string::const_iterator& end, int& depth);
	bool parseSubShaderPass(std::string::const_iterator& start, std::string::const_iterator& end, stPass& pass, int& depth);
	bool findEndingCurlyBraseOfModule(std::string::const_iterator& start, std::string::const_iterator& end, int& depth);
	bool parseSubShader_vertex(std::string::const_iterator& start, std::string::const_iterator& end, stPass& pass, int& depth);
	bool parseSubShader_fragment(std::string::const_iterator& start, std::string::const_iterator& end, stPass& pass, int& depth);
	bool parseSubShader_tag(std::string::const_iterator& start, std::string::const_iterator& end, stPass& pass, int& depth);
	bool parseShader(std::string::const_iterator& start, std::string::const_iterator& end, int& depth);
	bool parse(std::string::const_iterator& start, std::string::const_iterator& end, int& depth);

protected:
	stSubShader m_cSubShader;
	std::vector<stShaderProperty_Texture2D*> m_vTex2D_Properties;
	std::vector<stShaderProperty_Color*> m_vColor_Properties;
	std::vector<stShaderProperty_Range*> m_vRange_Properties;
	std::vector<stShaderProperty_Vector*> m_vVector_Properties;

	std::vector<stTextureMap*> m_vTextureMap;
	std::vector<gxHWShader*> m_vShaderProgram;
	std::string m_cName;
	std::string m_cFileName;
public:
	gxSurfaceShader();
	virtual ~gxSurfaceShader();

	const char* getName()	{	return m_cName.c_str();	}
	const char* getSurfaceShaderFileName()	{	return m_cFileName.c_str();	}
	bool loadSurfaceShader(const char* filename);

	void appendTextureMap(stTextureMap* texmap);

	int getShaderPassCount()				{	return m_vShaderProgram.size();	}
	gxHWShader* getShaderPass(int pass)		{	return (pass<m_vShaderProgram.size())?m_vShaderProgram[pass]:NULL;	}
	stPass* getShaderPassStruct(int pass)	{	return (pass<m_cSubShader.m_vPass.size())?m_cSubShader.m_vPass[pass]:NULL;}

	std::vector<stPass*>* getShaderPassList()	{	return &m_cSubShader.m_vPass;	}
	std::vector<stShaderProperty_Texture2D*>* getShaderPropertyList_Texture2D()	{	return &m_vTex2D_Properties;		}
	std::vector<stShaderProperty_Color*>* getShaderPropertyList_Color()			{	return &m_vColor_Properties;		}
	std::vector<stShaderProperty_Range*>* getShaderPropertyList_Range()			{	return &m_vRange_Properties;		}
	std::vector<stShaderProperty_Vector*>* getShaderPropertyList_Vector()		{	return &m_vVector_Properties;		}

	//stShaderProperty_Texture2D* getShaderProperty_Texture2D(int index)	{	return m_vTex2D_Properties[index];	}
	//stShaderProperty_Color* getShaderProperty_Color(int index)			{	return m_vColor_Properties[index];	}
};
#endif