#pragma once

#include <stdio.h>
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
		glslShader=NULL;
		shadow = 0;
	}

	~stPass()
	{
		vertexGLSLSourceBuffer.clear();
		fragmentGLSLSourceBuffer.clear();
		glslIncludeModuleList.clear();
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
	unsigned int shadow;	//0 - OFF, 1 - Normal

	std::string vertexGLSLSourceBuffer;
	std::string fragmentGLSLSourceBuffer;
	std::vector<std::string> glslIncludeModuleList;
	gxHWShader* glslShader;
};

//////////Surface Shader Properties//////////
struct stShaderPropertyBase
{
private:
	stShaderPropertyBase(){}
public:
	stShaderPropertyBase(int type):
		propertyID(type)
	{
		renderPass = NULL;
		uniformGLSLVariable=-1;
	}

	virtual ~stShaderPropertyBase()
	{
		name.clear();
		nameofProperty.clear();
	}
	
	virtual void sendToGLSL(bool bDoEnableAndDisableShader=false)=0;	//if bDoEnableAndDisableShader the do the enable/disable of shader inside sendToGLSL()

	stPass* renderPass;
	int propertyID;
	std::string name;	//sometimes used to identify inside glsl shader
	std::string nameofProperty;
	int uniformGLSLVariable;
};
//

struct stShaderProperty_Vector : public stShaderPropertyBase
{
	stShaderProperty_Vector()
		:stShaderPropertyBase(1)
	{
	}

	void sendToGLSL(bool bDoEnableAndDisableShader=false)
	{
		if(bDoEnableAndDisableShader)
			renderPass->glslShader->enableProgram();

		if(uniformGLSLVariable==-1)
		{
			uniformGLSLVariable = renderPass->glslShader->getUniformLoc(nameofProperty.c_str());
		}
		CHECK_GL_ERROR(glUniform4fv(uniformGLSLVariable, 1, &vector.x));

		if(bDoEnableAndDisableShader)
			renderPass->glslShader->disableProgram();
	}

	vector4f vector;
};

struct stShaderProperty_Range : public stShaderPropertyBase
{
	stShaderProperty_Range():
		stShaderPropertyBase(2)
	{
		range_max=range_min=range_value=0.0f;
	}

	void sendToGLSL(bool bDoEnableAndDisableShader=false)
	{
		if(bDoEnableAndDisableShader)
			renderPass->glslShader->enableProgram();

		if(uniformGLSLVariable==-1)
		{
			uniformGLSLVariable = renderPass->glslShader->getUniformLoc(nameofProperty.c_str());
		}
		CHECK_GL_ERROR(glUniform1f(uniformGLSLVariable, range_value));

		if(bDoEnableAndDisableShader)
			renderPass->glslShader->disableProgram();
	}

	float range_max;
	float range_min;
	float range_value;
};

struct stShaderProperty_Color : public stShaderPropertyBase
{
	stShaderProperty_Color()
		:stShaderPropertyBase(3)
	{
	}

	void sendToGLSL(bool bDoEnableAndDisableShader=false)
	{
		if(bDoEnableAndDisableShader)
			renderPass->glslShader->enableProgram();

		if(uniformGLSLVariable==-1)
		{
			uniformGLSLVariable = renderPass->glslShader->getUniformLoc(nameofProperty.c_str());
		}
		CHECK_GL_ERROR(glUniform4fv(uniformGLSLVariable, 1, &color.x));

		if(bDoEnableAndDisableShader)
			renderPass->glslShader->disableProgram();
	}

	vector4f color;
};

struct stShaderProperty_Texture2D : public stShaderPropertyBase
{
	stShaderProperty_Texture2D()
		:stShaderPropertyBase(4)
	{
	}

	~stShaderProperty_Texture2D()
	{
		texture_name.clear();
		texture_uv_in_name.clear();
		texture_uv_out_name.clear();
		texture_sampler2d_name.clear();
	}

	void sendToGLSL(bool bDoEnableAndDisableShader=false)
	{
	}

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

	void setTextureName(const char* name)	{	GX_STRCPY(textureName, name);	}
	void setTextureCRC(int crc)				{	textureCRC=crc;     }
	const char* getTextureName()			{	return textureName;	}
	int getTextureCRC()						{	return textureCRC;	}
    gxTexture* getTexture()                 {	return texture;     }
	gxTexture* load(CTextureManager& textureManager, const char* filename);
	gxTexture* loadTextureFromMeta(CTextureManager& textureManager, int crc);

	void setShaderTextureProperty(stShaderProperty_Texture2D* property_tex2D)	{	shaderPropertyVariable = property_tex2D;	}
	stShaderProperty_Texture2D* getShaderTextureProperty()						{	return shaderPropertyVariable;			}

private:
	char textureName[256];
	int textureCRC;
	gxTexture* texture;
	stShaderProperty_Texture2D* shaderPropertyVariable;
};

struct stSubShader
{
	~stSubShader()
	{
		for(std::vector<stPass*>::iterator it = renderPassList.begin(); it != renderPassList.end(); ++it)
		{
			stPass* pass = *it;
			GX_DELETE(pass);
		}
		renderPassList.clear();
	}

	std::vector<stPass*> renderPassList;
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

	void applyDefaultValuesOfPropertiesToGLSLShader(stPass* currentPass);

protected:
	stSubShader subShader;
	std::vector<stShaderProperty_Texture2D*> texture2DPropertyList;
	std::vector<stShaderProperty_Color*> colorPropertyList;
	std::vector<stShaderProperty_Range*> rangePropertyList;
	std::vector<stShaderProperty_Vector*> vectorPropertyList;

	std::vector<stTextureMap*> textureMapList;
	std::vector<gxHWShader*> shaderProgramList;
	std::string m_cName;
	std::string surfaceShaderFileName;
    
public:
	gxSurfaceShader();
	virtual ~gxSurfaceShader();

	const char* getName()	{	return m_cName.c_str();	}
	const char* getSurfaceShaderFileName()	{	return surfaceShaderFileName.c_str();	}
	bool loadSurfaceShader(const char* filename);

	void appendTextureMap(stTextureMap* texmap);

	int getShaderPassCount()				{	return (int)shaderProgramList.size();	}
	gxHWShader* getShaderPass(int pass)		{	return (pass<shaderProgramList.size())?shaderProgramList[pass]:NULL;	}
	stPass* getShaderPassStruct(int pass)	{	return (pass<subShader.renderPassList.size())?subShader.renderPassList[pass]:NULL;}

	std::vector<stPass*>* getShaderPassList()	{	return &subShader.renderPassList;	}
	std::vector<stShaderProperty_Texture2D*>* getShaderPropertyList_Texture2D()	{	return &texture2DPropertyList;		}
	std::vector<stShaderProperty_Color*>* getShaderPropertyList_Color()			{	return &colorPropertyList;		}
	std::vector<stShaderProperty_Range*>* getShaderPropertyList_Range()			{	return &rangePropertyList;		}
	std::vector<stShaderProperty_Vector*>* getShaderPropertyList_Vector()		{	return &vectorPropertyList;		}
};