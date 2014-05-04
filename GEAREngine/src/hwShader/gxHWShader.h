#ifndef GXHWSHADER_H
#define GXHWSHADER_H

#include "../renderer/gxRenderer.h"
#include "../core/gxDebug.h"
#include <vector>
#include "../core/object3d.h"
#include <iostream>	//included to support 'std::string' in android

class object3d;
struct stUniformLocation
{
	stUniformLocation()
	{
		uniform_ref=-1;
		memset(uniform_ref_name, 0, sizeof(uniform_ref_name));
	}

	int uniform_ref;
	char uniform_ref_name[64];	//uniform vars in shaders must not exceed 64 charachters
};

struct stAttribLocation
{
	stAttribLocation()
	{
		attrib_ref=-1;
		memset(attrib_ref_name, 0, sizeof(attrib_ref_name));
	}

	int attrib_ref;
	char attrib_ref_name[64];	//uniform vars in shaders must not exceed 64 charachters
};

class DllExport gxHWShader
{
public:
	gxHWShader();
	virtual ~gxHWShader();

	bool loadShader(const char* shaderFile);
	bool loadShaderFromBuffer(const char* name, const char* shaderBuffer, int shaderBuffer_size);
	bool compileShader(GLuint* shader, GLenum type, const char* source, int fileSz);
	bool linkProgram();
	
	void attachShader();
	void detachShader();
	void destroyShader();
	void enableProgram();
	void disableProgram();
	bool validateProgram();
	
	GLuint getProgram()			{	return m_cProgram;		}
	GLuint getVertexShader()	{	return m_cVertShader;	}
	GLuint getFragmentShader()	{	return m_cFragShader;	}
	
	void showShaderLog(const char* title);

	int getUniformLoc(const char* uvar);
	virtual int getAttribLoc(const char* uvar);

	void sendUniform1f(const char* name, float x);
	void sendUniform2f(const char* name, float x, float y);
	void sendUniform3f(const char* name, float x, float y, float z);
	void sendUniform4f(const char* name, float x, float y, float z, float w);
	void sendUniform1i(const char* name, int x);
	void sendUniform2i(const char* name, int x, int y);
	void sendUniform3i(const char* name, int x, int y, int z);
	void sendUniform4i(const char* name, int x, int y, int z, int w);
	void sendUniformTMfv(const char* name, const float* matrix, bool transpose, int size);
	void sendUniform2fv(const char* name, const float* input);
	void sendUniform3fv(const char* name, const float* input);
	void sendUniform4fv(const char* name, const float* input);
	void sendUniform4fv(const char* name, const float* input, int nVectors);

	void sendAttrib2fv(const char* name, const float* input);
	void sendAttrib3fv(const char* name, const float* input);
	void sendAttrib4fv(const char* name, const float* input);
	void sendAttrib1f(const char* name, float x);
    
	const char* getShaderName()	{	return m_cShaderName.c_str(); }

	//predefined vars
	void sendUniform_GEAR_MVP(const float* input);
	void sendUniform_GEAR_MODEL_MATRIX(const float* input);
	void sendUniform_GEAR_MODEL_INVERSE(const float* input);
	void sendUniform_material_diffuse(const float* input);
	void sendUniform_material_ambient(const float* input);
	void sendUniform_material_specular(const float* input);
	void sendUniform_material_shininess(float input);

	int getAttrib_vIN_Position();
	int getAttrib_vIN_Normal();
	int getAttrib_Tangent();

private:
	void clearUniformRefVarList();
	void clearAttribRefVarList();

    GLuint m_cProgram;
	GLuint m_cVertShader;
	GLuint m_cFragShader;

	std::vector<stUniformLocation*> m_vUniformRefVarList;
	std::vector<stAttribLocation*> m_vAttribRefVarList;
	std::string m_cShaderName;

	//predefined vars
	int m_cUnifrom_GEAR_MVP;
	int m_cUnifrom_GEAR_MODEL_MATRIX;
	int m_cUnifrom_GEAR_MODEL_INVERSE;
	int m_cUnifrom_material_diffuse;
	int m_cUnifrom_material_ambient;
	int m_cUnifrom_material_specular;
	int m_cUnifrom_material_shininess;

	int m_cAttrib_vIN_Position;
	int m_cAttrib_vIN_Normal;
	int m_cAttrib_Tangent;
};

#endif
