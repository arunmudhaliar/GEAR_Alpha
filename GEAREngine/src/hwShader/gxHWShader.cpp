#include "gxHWShader.h"
//#include "../util/objC_util.h"

gxHWShader::gxHWShader()
{
	m_cProgram=0;
	m_cVertShader=0;
	m_cFragShader=0;

	//predefined vars
	m_cUnifrom_GEAR_MODELVIEW=-1;
	m_cUnifrom_GEAR_MVP=-1;
	m_cUnifrom_GEAR_MODEL_MATRIX=-1;
	m_cUnifrom_GEAR_MODEL_INVERSE=-1;
	m_cUnifrom_material_diffuse=-1;
	m_cUnifrom_material_ambient=-1;
	m_cUnifrom_material_specular=-1;
	m_cUnifrom_material_shininess=-1;
	m_cUnifrom_GEAR_Time=-1;
	m_cUnifrom_GEAR_ScreenParams=-1;

	m_cAttrib_vIN_Position=-1;
	m_cAttrib_vIN_Normal=-1;
	m_cAttrib_Tangent=-1;
	//

}

gxHWShader::~gxHWShader()
{
	clearAttribRefVarList();
	clearUniformRefVarList();
}

bool gxHWShader::loadShaderFromBuffer(const char* name, const char* shaderBuffer, int shaderBuffer_size)
{
	const char* define_vertex="#define GEAR_VERTEX_SHADER\n";
	const char* define_fragment="#define GEAR_FRAGMENT_SHADER\n";

	int vSz=shaderBuffer_size+(int)strlen(define_vertex);
	char* vsource=new char[vSz];
	memset((void*)vsource, 0, vSz);
	strcpy(vsource, define_vertex);
	strncpy(&vsource[strlen(define_vertex)], shaderBuffer, shaderBuffer_size);

	int fSz=shaderBuffer_size+(int)strlen(define_fragment);
	char* fsource=new char[fSz];
	memset((void*)fsource, 0, fSz);
	strcpy(fsource, define_fragment);
	strncpy(&fsource[strlen(define_fragment)], shaderBuffer, shaderBuffer_size);

	// Create and compile vertex shader
	if (!compileShader(&m_cVertShader, GL_VERTEX_SHADER, vsource, vSz))
	{
		DEBUG_PRINT("Failed to compile vertex shader from buffer");
		GX_DELETE_ARY(vsource);
		GX_DELETE_ARY(fsource);
		return false;
	}
	
	// Create and compile fragment shader
	if (!compileShader(&m_cFragShader, GL_FRAGMENT_SHADER, fsource, fSz))
	{
		DEBUG_PRINT("Failed to compile fragment shader from buffer");
		GX_DELETE_ARY(vsource);
		GX_DELETE_ARY(fsource);
		return false;
	}
	
	GX_DELETE_ARY(vsource);
	GX_DELETE_ARY(fsource);

	// Create shader program
	m_cProgram = glCreateProgram();

	attachShader();
	
	// Link program
	if (!linkProgram())
	{
		DEBUG_PRINT("Failed to link program: %d from buffer", m_cProgram);
		destroyShader();
		GX_DELETE_ARY(vsource);
		GX_DELETE_ARY(fsource);
		return false;
	}
	
	validateProgram();
    detachShader();
	disableProgram();
	
	m_cShaderName.assign(name);

	return true;
}

bool gxHWShader::loadShader(const char* shaderFile)
{
	DEBUG_PRINT("Load shader %s", shaderFile);

	//read shader code
	int fileSz=0;
	FILE* fp=fopen(shaderFile, "r");
	if(fp==NULL)
	{
		DEBUG_PRINT("Shader %s FILE open ERROR #1", shaderFile);
		return false;
	}

	fseek(fp, 0, SEEK_END);
	fileSz=(int)ftell(fp);
	fclose(fp);
	
	if(!fileSz)
	{
		DEBUG_PRINT("Shader %s FILE size ZERO ERROR", shaderFile);
		return false;
	}
	
#ifdef ANDROID
	const char* define_vertex="#define GEAR_VERTEX_SHADER\n";
#else
	const char* define_vertex="#version 120\n#define GEAR_VERTEX_SHADER\n";
#endif
	const char* define_fragment="#define GEAR_FRAGMENT_SHADER\n";

	//vertex shader source
	fp=fopen(shaderFile, "r");
	if(fp==NULL)
	{
		DEBUG_PRINT("Shader %s FILE open ERROR #2", shaderFile);
		return false;
	}
	int vSz=fileSz+(int)strlen(define_vertex);
	char* vsource=new char[vSz];
	memset((void*)vsource, 0, vSz);
	strcpy(vsource, define_vertex);

	fread((void*)&vsource[strlen(define_vertex)], 1, fileSz, fp);
	fclose(fp);
	//

	//fragment shader source
	fp=fopen(shaderFile, "r");
	if(fp==NULL)
	{
		DEBUG_PRINT("Shader %s FILE open ERROR #3", shaderFile);
		return false;
	}
	int fSz=fileSz+(int)strlen(define_fragment);
	char* fsource=new char[fSz];
	memset((void*)fsource, 0, fSz);
	strcpy(fsource, define_fragment);

	fread((void*)&fsource[strlen(define_fragment)], 1, fileSz, fp);
	fclose(fp);
	//
	
	// Create and compile vertex shader
	if (!compileShader(&m_cVertShader, GL_VERTEX_SHADER, vsource, vSz))
	{
		DEBUG_PRINT("Failed to compile vertex shader '%s'", shaderFile);
		GX_DELETE_ARY(vsource);
		GX_DELETE_ARY(fsource);
		return false;
	}
	
	// Create and compile fragment shader
	if (!compileShader(&m_cFragShader, GL_FRAGMENT_SHADER, fsource, fSz))
	{
		DEBUG_PRINT("Failed to compile fragment shader '%s'", shaderFile);
		GX_DELETE_ARY(vsource);
		GX_DELETE_ARY(fsource);
		return false;
	}
	
	GX_DELETE_ARY(vsource);
	GX_DELETE_ARY(fsource);

	// Create shader program
	m_cProgram = glCreateProgram();

	attachShader();
	
	// Link program
	if (!linkProgram())
	{
		DEBUG_PRINT("Failed to link program: %d (shader : %s)", m_cProgram, shaderFile);
		destroyShader();
		GX_DELETE_ARY(vsource);
		GX_DELETE_ARY(fsource);
		return false;
	}
	
	validateProgram();
    detachShader();
	disableProgram();
	
	m_cShaderName.assign(shaderFile);

	DEBUG_PRINT("Shader Loaded %s", shaderFile);

	return true;
}

bool gxHWShader::compileShader(GLuint* shader, GLenum type, const char* source, int fileSz)
{
    GLint status;

    *shader = glCreateShader(type);
    CHECK_GL_ERROR(glShaderSource(*shader, 1, &source, &fileSz));
    CHECK_GL_ERROR(glCompileShader(*shader));
	
#if defined(DEBUG) || defined(_DEBUG)	//DEBUG for MAC & _DEBUG for Win32
	GLint logLength;
    glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 1)
    {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetShaderInfoLog(*shader, logLength, &logLength, log);
        DEBUG_PRINT("Shader compile log:\n%s", log);
        free(log);
    }
#endif
	
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);
    if (status == 0)
    {
        CHECK_GL_ERROR(glDeleteShader(*shader));
        return false;
    }
	
    return true;
}

bool gxHWShader::linkProgram()
{
    GLint status;
	
    CHECK_GL_ERROR(glLinkProgram(m_cProgram));
	
#if defined(DEBUG) || defined(_DEBUG)	//DEBUG for MAC & _DEBUG for Win32
    GLint logLength;
    glGetProgramiv(m_cProgram, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 1)
    {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetProgramInfoLog(m_cProgram, logLength, &logLength, log);
        DEBUG_PRINT("Program link log:\n%s", log);
        free(log);
    }
#endif
	
    CHECK_GL_ERROR(glGetProgramiv(m_cProgram, GL_LINK_STATUS, &status));
    if (status == 0)
        return false;
	
    return true;
}

void gxHWShader::attachShader()
{
	// Attach vertex shader to program
	CHECK_GL_ERROR(glAttachShader(m_cProgram, m_cVertShader));
	// Attach fragment shader to program
	CHECK_GL_ERROR(glAttachShader(m_cProgram, m_cFragShader));	
}

void gxHWShader::detachShader()
{
	// Release vertex and fragment shaders
	if (m_cVertShader)
		CHECK_GL_ERROR(glDetachShader(m_cProgram, m_cVertShader));
	if (m_cFragShader)
		CHECK_GL_ERROR(glDetachShader(m_cProgram, m_cFragShader));

	clearAttribRefVarList();
	clearUniformRefVarList();
}

void gxHWShader::destroyShader()
{
	detachShader();

	if (m_cVertShader)
	{
		CHECK_GL_ERROR(glDeleteShader(m_cVertShader));
		m_cVertShader = 0;
	}
	if (m_cFragShader)
	{
		CHECK_GL_ERROR(glDeleteShader(m_cFragShader));
		m_cFragShader = 0;
	}
	if (m_cProgram)
	{
		CHECK_GL_ERROR(glDeleteProgram(m_cProgram));
		m_cProgram = 0;
	}	
}

void gxHWShader::enableProgram()
{
	// Use shader program
    CHECK_GL_ERROR(glUseProgram(m_cProgram));
}

void gxHWShader::disableProgram()
{
	CHECK_GL_ERROR(glUseProgram(0));
}

bool gxHWShader::validateProgram()
{
	GLint logLength, status;
	
	CHECK_GL_ERROR(glValidateProgram(m_cProgram));

#if defined(DEBUG) || defined(_DEBUG)	//DEBUG for MAC & _DEBUG for Win32
	glGetProgramiv(m_cProgram, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 1)
	{
		GLchar *log = (GLchar *)malloc(logLength);
		glGetProgramInfoLog(m_cProgram, logLength, &logLength, log);
		DEBUG_PRINT("Program validate log:\n%s", log);
		free(log);
	}
#endif

	CHECK_GL_ERROR(glGetProgramiv(m_cProgram, GL_VALIDATE_STATUS, &status));
	if (status == 0)
		return false;
	
	return true;
}

void gxHWShader::showShaderLog(const char* title)
{
	GLint logLength;
	CHECK_GL_ERROR(glGetProgramiv(m_cProgram, GL_INFO_LOG_LENGTH, &logLength));
	if (logLength > 1)
	{
		GLchar *log = (GLchar *)malloc(logLength);
		CHECK_GL_ERROR(glGetProgramInfoLog(m_cProgram, logLength, &logLength, log));
		DEBUG_PRINT("title :%s\n%s", title, log);
		free(log);
	}
}

int gxHWShader::getAttribLoc(const char* avar)
{
	for(std::vector<stAttribLocation*>::iterator it = m_vAttribRefVarList.begin(); it != m_vAttribRefVarList.end(); ++it)
	{
		stAttribLocation* attribRefVar = *it;
		if(strcmp(attribRefVar->attrib_ref_name, avar)==0)
		{
			return attribRefVar->attrib_ref;
		}
	}

	int loc = glGetAttribLocation(m_cProgram, avar);
	if(loc == -1)
	{
		DEBUG_PRINT("getAttribLoc() returned -1 for %s", avar);
		return -1;
	}

	stAttribLocation* newAttribRefVar=new stAttribLocation();
	GX_STRCPY(newAttribRefVar->attrib_ref_name, avar);
	newAttribRefVar->attrib_ref=loc;

	m_vAttribRefVarList.push_back(newAttribRefVar);

	return loc;
}

void gxHWShader::clearAttribRefVarList()
{
	for(std::vector<stAttribLocation*>::iterator it = m_vAttribRefVarList.begin(); it != m_vAttribRefVarList.end(); ++it)
	{
		stAttribLocation* attribRefVar = *it;
		GX_DELETE(attribRefVar);
	}

	m_vAttribRefVarList.clear();
}

int gxHWShader::getUniformLoc(const char* uvar)
{
	for(std::vector<stUniformLocation*>::iterator it = m_vUniformRefVarList.begin(); it != m_vUniformRefVarList.end(); ++it)
	{
		stUniformLocation* uniformRefVarnode = *it;
		if(strcmp(uniformRefVarnode->uniform_ref_name, uvar)==0)
		{
			return uniformRefVarnode->uniform_ref;
		}
	}

	int loc = glGetUniformLocation(m_cProgram, uvar);
	if(loc == -1)
	{
		DEBUG_PRINT("getUniformLoc() returned -1 for %s", uvar);
		return -1;
	}

	stUniformLocation* newUnifromRefVar=new stUniformLocation();
	GX_STRCPY(newUnifromRefVar->uniform_ref_name, uvar);
	newUnifromRefVar->uniform_ref=loc;

	m_vUniformRefVarList.push_back(newUnifromRefVar);

	return loc;
}

void gxHWShader::clearUniformRefVarList()
{
	for(std::vector<stUniformLocation*>::iterator it = m_vUniformRefVarList.begin(); it != m_vUniformRefVarList.end(); ++it)
	{
		stUniformLocation* uniformRefVarnode = *it;
		GX_DELETE(uniformRefVarnode);
	}

	m_vUniformRefVarList.clear();
}

void gxHWShader::sendUniform1f(const char* name, float x)
{
	CHECK_GL_ERROR(glUniform1f(getUniformLoc(name), x));
}

void gxHWShader::sendUniform2f(const char* name, float x, float y)
{
	CHECK_GL_ERROR(glUniform2f(getUniformLoc(name), x, y));
}

void gxHWShader::sendUniform3f(const char* name, float x, float y, float z)
{
	CHECK_GL_ERROR(glUniform3f(getUniformLoc(name), x, y, z));
}

void gxHWShader::sendUniform4f(const char* name, float x, float y, float z, float w)
{
	CHECK_GL_ERROR(glUniform4f(getUniformLoc(name), x, y, z, w));
}

void gxHWShader::sendUniform1i(const char* name, int x)
{
	CHECK_GL_ERROR(glUniform1i(getUniformLoc(name), x));
}

void gxHWShader::sendUniform2i(const char* name, int x, int y)
{
	CHECK_GL_ERROR(glUniform2i(getUniformLoc(name), x, y));
}

void gxHWShader::sendUniform3i(const char* name, int x, int y, int z)
{
	CHECK_GL_ERROR(glUniform3i(getUniformLoc(name), x, y, z));
}

void gxHWShader::sendUniform4i(const char* name, int x, int y, int z, int w)
{
	CHECK_GL_ERROR(glUniform4i(getUniformLoc(name), x, y, z, w));
}

void gxHWShader::sendUniformTMfv(const char* name, const float* matrix, bool transpose, int size)
{
	int loc = getUniformLoc(name);
  
	switch(size)
    {
    case 2:
		CHECK_GL_ERROR(glUniformMatrix2fv(loc, 1, transpose, matrix));
		break;
    case 3:
		CHECK_GL_ERROR(glUniformMatrix3fv(loc, 1, transpose, matrix));
		break;
    case 4:
		CHECK_GL_ERROR(glUniformMatrix4fv(loc, 1, transpose, matrix));
		break;
    }
}

void gxHWShader::sendUniform2fv(const char* name, const float* input)
{
	CHECK_GL_ERROR(glUniform2fv(getUniformLoc(name), 1, input));
}

void gxHWShader::sendUniform3fv(const char* name, const float* input)
{
	CHECK_GL_ERROR(glUniform3fv(getUniformLoc(name), 1, input));
}

void gxHWShader::sendUniform4fv(const char* name, const float* input)
{
	CHECK_GL_ERROR(glUniform4fv(getUniformLoc(name), 1, input));
}

void gxHWShader::sendUniform4fv(const char* name, const float* input, int nVectors)
{
    CHECK_GL_ERROR(glUniform4fv(getUniformLoc(name), nVectors, input));
}


//=========================ATTRIB=======================
void gxHWShader::sendAttrib2fv(const char* name, const float* input)
{
	int loc=getAttribLoc(name);
	CHECK_GL_ERROR(glVertexAttrib2fv(loc, input));
}

void gxHWShader::sendAttrib3fv(const char* name, const float* input)
{
	int loc=getAttribLoc(name);
	CHECK_GL_ERROR(glVertexAttrib3fv(loc, input));
}

void gxHWShader::sendAttrib4fv(const char* name, const float* input)
{
	int loc=getAttribLoc(name);
	CHECK_GL_ERROR(glVertexAttrib4fv(loc, input));
}

void gxHWShader::sendAttrib1f(const char* name, float x)
{
	int loc=getAttribLoc(name);
	CHECK_GL_ERROR(glVertexAttrib1f(loc, x));
}

//predefined vars
void gxHWShader::sendUniform_GEAR_MODELVIEW(const float* input)
{
	if(m_cUnifrom_GEAR_MODELVIEW==-1)
	{
		m_cUnifrom_GEAR_MODELVIEW = getUniformLoc("GEAR_MODELVIEW");
	}
	
	CHECK_GL_ERROR(glUniformMatrix4fv(m_cUnifrom_GEAR_MODELVIEW, 1, false, input));
}

void gxHWShader::sendUniform_GEAR_MVP(const float* input)
{
	if(m_cUnifrom_GEAR_MVP==-1)
	{
		m_cUnifrom_GEAR_MVP = getUniformLoc("GEAR_MVP");
	}

	CHECK_GL_ERROR(glUniformMatrix4fv(m_cUnifrom_GEAR_MVP, 1, false, input));
}

void gxHWShader::sendUniform_GEAR_MODEL_MATRIX(const float* input)
{
	if(m_cUnifrom_GEAR_MODEL_MATRIX==-1)
	{
		m_cUnifrom_GEAR_MODEL_MATRIX = getUniformLoc("GEAR_MODEL_MATRIX");
	}
	
	CHECK_GL_ERROR(glUniformMatrix4fv(m_cUnifrom_GEAR_MODEL_MATRIX, 1, false, input));
}

void gxHWShader::sendUniform_GEAR_MODEL_INVERSE(const float* input)
{
	if(m_cUnifrom_GEAR_MODEL_INVERSE==-1)
	{
		m_cUnifrom_GEAR_MODEL_INVERSE = getUniformLoc("GEAR_MODEL_INVERSE");
	}
		
	CHECK_GL_ERROR(glUniformMatrix4fv(m_cUnifrom_GEAR_MODEL_INVERSE, 1, false, input));
}

void gxHWShader::sendUniform_material_diffuse(const float* input)
{
	if(m_cUnifrom_material_diffuse==-1)
	{
		m_cUnifrom_material_diffuse = getUniformLoc("material.diffuse");
	}

	CHECK_GL_ERROR(glUniform4fv(m_cUnifrom_material_diffuse, 1, input));
}

void gxHWShader::sendUniform_material_ambient(const float* input)
{
	if(m_cUnifrom_material_ambient==-1)
	{
		m_cUnifrom_material_ambient = getUniformLoc("material.ambient");
	}
	
	CHECK_GL_ERROR(glUniform4fv(m_cUnifrom_material_ambient, 1, input));
}

void gxHWShader::sendUniform_material_specular(const float* input)
{
	if(m_cUnifrom_material_specular==-1)
	{
		m_cUnifrom_material_specular = getUniformLoc("material.specular");
	}
	
	CHECK_GL_ERROR(glUniform4fv(m_cUnifrom_material_specular, 1, input));
}

void gxHWShader::sendUniform_material_shininess(float input)
{
	if(m_cUnifrom_material_shininess==-1)
	{
		m_cUnifrom_material_shininess = getUniformLoc("material.shininess");
	}

	CHECK_GL_ERROR(glUniform1f(m_cUnifrom_material_shininess, input));
}

void gxHWShader::sendUniform_GEAR_Time(const float* time)
{
	if(m_cUnifrom_GEAR_Time==-1)
	{
		m_cUnifrom_GEAR_Time = getUniformLoc("GEAR_Time");
	}
	
	CHECK_GL_ERROR(glUniform4fv(m_cUnifrom_GEAR_Time, 1, time));
}

void gxHWShader::sendUniform_GEAR_ScreenParams(const float* screenParams)
{
	if(m_cUnifrom_GEAR_ScreenParams==-1)
	{
		m_cUnifrom_GEAR_ScreenParams = getUniformLoc("GEAR_ScreenParams");
	}

	CHECK_GL_ERROR(glUniform4fv(m_cUnifrom_GEAR_ScreenParams, 1, screenParams));
}

int gxHWShader::getAttrib_vIN_Position()
{
	if(m_cAttrib_vIN_Position==-1)
	{
		m_cAttrib_vIN_Position=getAttribLoc("vIN_Position");
	}
	return m_cAttrib_vIN_Position;
}

int gxHWShader::getAttrib_vIN_Normal()
{
	if(m_cAttrib_vIN_Normal==-1)
	{
		m_cAttrib_vIN_Normal=getAttribLoc("vIN_Normal");
	}
	return m_cAttrib_vIN_Normal;
}

int gxHWShader::getAttrib_Tangent()
{
	if(m_cAttrib_Tangent==-1)
	{
		m_cAttrib_Tangent=getAttribLoc("Tangent");
	}
	return m_cAttrib_Tangent;
}