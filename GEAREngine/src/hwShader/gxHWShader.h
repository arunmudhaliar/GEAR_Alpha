#ifndef GXHWSHADER_H
#define GXHWSHADER_H

#include "../renderer/gxRenderer.h"
#include "../core/gxDebug.h"
#include <vector>
#include "../core/object3d.h"

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

class __declspec( dllexport ) gxHWShader
{
public:

	enum EHWShaderFlags
	{
		OBJBASE_FLAG_USE_TM                     = 0,		//if it is set the object is dynamic
		//material
		MATERIAL_FLAG_USE_AMBIENT_MAP			= 2,
		MATERIAL_FLAG_USE_DIFFUSE_MAP			= 3,
		MATERIAL_FLAG_USE_SPECULAR_CLR_MAP      = 4,
		MATERIAL_FLAG_USE_SPECULAR_MAP          = 5,
		MATERIAL_FLAG_USE_GLOSSINESS_MAP		= 6,
		MATERIAL_FLAG_USE_SELF_ILLUMINATION_MAP	= 7,
		MATERIAL_FLAG_USE_OPACITY_MAP			= 8,
		MATERIAL_FLAG_USE_FILTER_CLR_MAP		= 9,
		MATERIAL_FLAG_USE_NORMAL_MAP			= 10,
		MATERIAL_FLAG_USE_REFLECTION_MAP		= 11,
		MATERIAL_FLAG_USE_REFRACTION_MAP		= 12,
        //mesh
        //MESH_FLAG_USE_
		//lighting
		OTHER_FLAG_USE_LIGHTING		= 15    //last flag
	};

	gxHWShader();
	virtual ~gxHWShader();

	bool loadShader(const char* shaderFile);
	bool compileShader(GLuint* shader, GLenum type, const char* source, int fileSz);
	bool linkProgram();
	
	void attachShader();
	void detachShader();
	virtual void bind()=0;
	virtual void getUniformVars()=0;
	void destroyShader();
	void enableProgram();
	void disableProgram();

	virtual void updateShaderVars(float dt)=0;
	virtual void inputShaderUniformVars(void* ptr)=0;
	virtual void inputShaderAttribVars(void* ptr)=0;
	
	virtual GLint getUserDefinedUniform(int index)=0;
	
	bool validateProgram();
	
	GLuint getProgram()			{	return m_cProgram;		}
	GLuint getVertexShader()	{	return m_cVertShader;	}
	GLuint getFragmentShader()	{	return m_cFragShader;	}
	
	virtual void renderThroughHWShader(object3d* obj, const matrix4x4f* parentTM)=0;

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


    void resetAllFlags();
    void setFlag(EHWShaderFlags eFlag, float val);
    void resetFlag(EHWShaderFlags eFlag);
    float getFlag(EHWShaderFlags eFlag);
    float* getFlagPtr() {   return m_cszFlags;  }
    
private:

	void clearUniformRefVarList();
	void clearAttribRefVarList();

    GLuint m_cProgram;
	GLuint m_cVertShader;
	GLuint m_cFragShader;

	std::vector<stUniformLocation*> m_vUniformRefVarList;
	std::vector<stAttribLocation*> m_vAttribRefVarList;

    float m_cszFlags[16];
};

#endif
