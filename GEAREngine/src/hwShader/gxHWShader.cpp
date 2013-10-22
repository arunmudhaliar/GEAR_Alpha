#include "gxHWShader.h"
//#include "../util/objC_util.h"

gxHWShader::gxHWShader()/*:
gxShader()*/
{
	m_cProgram=0;
	m_cVertShader=0;
	m_cFragShader=0;
    resetAllFlags();
}

gxHWShader::~gxHWShader()
{
	clearAttribRefVarList();
	clearUniformRefVarList();
}

bool gxHWShader::loadShader(const char* shaderFile)
{
	//read shader code
	int fileSz=0;
	FILE* fp=fopen(shaderFile, "r");
	if(fp==NULL) return false;

	fseek(fp, 0, SEEK_END);
	fileSz=ftell(fp);
	fclose(fp);
	
	if(!fileSz) return false;
	
	const char* define_vertex="#define GEAR_VERTEX_SHADER\n";
	const char* define_fragment="#define GEAR_FRAGMENT_SHADER\n";

	//vertex shader source
	fp=fopen(shaderFile, "r");
	if(fp==NULL) return false;
	int vSz=fileSz+strlen(define_vertex);
	char* vsource=new char[vSz];
	memset((void*)vsource, 0, vSz);
	strcpy(vsource, define_vertex);

	fread((void*)&vsource[strlen(define_vertex)], 1, fileSz, fp);
	fclose(fp);
	//

	//fragment shader source
	fp=fopen(shaderFile, "r");
	if(fp==NULL) return false;
	int fSz=fileSz+strlen(define_fragment);
	char* fsource=new char[fSz];
	memset((void*)fsource, 0, fSz);
	strcpy(fsource, define_fragment);

	fread((void*)&fsource[strlen(define_fragment)], 1, fileSz, fp);
	fclose(fp);
	//


	// Create shader program
	m_cProgram = glCreateProgram();
	
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

	attachShader();
	bind();
	
	// Link program
	if (!linkProgram())
	{
		DEBUG_PRINT("Failed to link program: %d (shader : %s)", m_cProgram, shaderFile);
		destroyShader();
		GX_DELETE_ARY(vsource);
		GX_DELETE_ARY(fsource);
		return false;
	}
	
	getUniformVars();
	
	validateProgram();

    detachShader();
    
	disableProgram();
	
	return true;
}

bool gxHWShader::compileShader(GLuint* shader, GLenum type, const char* source, int fileSz)
{
    GLint status;

    *shader = glCreateShader(type);
    glShaderSource(*shader, 1, &source, &fileSz);
    glCompileShader(*shader);
	
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
        glDeleteShader(*shader);
        return false;
    }
	
    return true;
}

bool gxHWShader::linkProgram()
{
    GLint status;
	
    glLinkProgram(m_cProgram);
	
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
	
    glGetProgramiv(m_cProgram, GL_LINK_STATUS, &status);
    if (status == 0)
        return false;
	
    return true;
}

void gxHWShader::attachShader()
{
	// Attach vertex shader to program
	glAttachShader(m_cProgram, m_cVertShader);
	// Attach fragment shader to program
	glAttachShader(m_cProgram, m_cFragShader);	
}

void gxHWShader::detachShader()
{
	// Release vertex and fragment shaders
	if (m_cVertShader)
		glDetachShader(m_cProgram, m_cVertShader);
	if (m_cFragShader)
		glDetachShader(m_cProgram, m_cFragShader);

	clearAttribRefVarList();
	clearUniformRefVarList();
}

//void gxHWShader::bind()
//{
//	//must be implemented by derived class
//	DEBUG_PRINT("must be implemented by derived class");
//}
//
//void gxHWShader::getUniformVars()
//{
//	//must be implemented by derived class
//	DEBUG_PRINT("must be implemented by derived class");
//}

void gxHWShader::destroyShader()
{
	detachShader();

	if (m_cVertShader)
	{
		glDeleteShader(m_cVertShader);
		m_cVertShader = 0;
	}
	if (m_cFragShader)
	{
		glDeleteShader(m_cFragShader);
		m_cFragShader = 0;
	}
	if (m_cProgram)
	{
		glDeleteProgram(m_cProgram);
		m_cProgram = 0;
	}	
}

void gxHWShader::enableProgram()
{
	// Use shader program
    glUseProgram(m_cProgram);
}

void gxHWShader::disableProgram()
{
	glUseProgram(0);
}

bool gxHWShader::validateProgram()
{
	GLint logLength, status;
	
	glValidateProgram(m_cProgram);

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

	glGetProgramiv(m_cProgram, GL_VALIDATE_STATUS, &status);
	if (status == 0)
		return false;
	
	return true;
}

void gxHWShader::resetAllFlags()
{
    memset(m_cszFlags, 0, sizeof(m_cszFlags));
}

void gxHWShader::setFlag(EHWShaderFlags eFlag, float val)
{
    m_cszFlags[eFlag]=val;
}

void gxHWShader::resetFlag(EHWShaderFlags eFlag)
{
    m_cszFlags[eFlag]=0.0f;
}

float gxHWShader::getFlag(EHWShaderFlags eFlag)
{
    return m_cszFlags[eFlag];
}

void gxHWShader::showShaderLog(const char* title)
{
	GLint logLength;
	glGetProgramiv(m_cProgram, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 1)
	{
		GLchar *log = (GLchar *)malloc(logLength);
		glGetProgramInfoLog(m_cProgram, logLength, &logLength, log);
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
	glUniform1f(getUniformLoc(name), x);
}

void gxHWShader::sendUniform2f(const char* name, float x, float y)
{
	glUniform2f(getUniformLoc(name), x, y);
}

void gxHWShader::sendUniform3f(const char* name, float x, float y, float z)
{
	glUniform3f(getUniformLoc(name), x, y, z);
}

void gxHWShader::sendUniform4f(const char* name, float x, float y, float z, float w)
{
	glUniform4f(getUniformLoc(name), x, y, z, w);
}

void gxHWShader::sendUniform1i(const char* name, int x)
{
	glUniform1i(getUniformLoc(name), x);
}

void gxHWShader::sendUniform2i(const char* name, int x, int y)
{
	glUniform2i(getUniformLoc(name), x, y);
}

void gxHWShader::sendUniform3i(const char* name, int x, int y, int z)
{
	glUniform3i(getUniformLoc(name), x, y, z);
}

void gxHWShader::sendUniform4i(const char* name, int x, int y, int z, int w)
{
	glUniform4i(getUniformLoc(name), x, y, z, w);
}

void gxHWShader::sendUniformTMfv(const char* name, const float* matrix, bool transpose, int size)
{
	int loc = getUniformLoc(name);
  
	switch(size)
    {
    case 2:
		glUniformMatrix2fv(loc, 1, transpose, matrix);
		break;
    case 3:
		glUniformMatrix3fv(loc, 1, transpose, matrix);
		break;
    case 4:
		glUniformMatrix4fv(loc, 1, transpose, matrix);
		break;
    }
}

void gxHWShader::sendUniform2fv(const char* name, const float* input)
{
	glUniform2fv(getUniformLoc(name), 1, input);
}

void gxHWShader::sendUniform3fv(const char* name, const float* input)
{
	glUniform3fv(getUniformLoc(name), 1, input);
}

void gxHWShader::sendUniform4fv(const char* name, const float* input)
{
	glUniform4fv(getUniformLoc(name), 1, input);
}

void gxHWShader::sendUniform4fv(const char* name, const float* input, int nVectors)
{
    glUniform4fv(getUniformLoc(name), nVectors, input);
}


//=========================ATTRIB=======================
void gxHWShader::sendAttrib2fv(const char* name, const float* input)
{
	int loc=getAttribLoc(name);
	glVertexAttrib2fv(loc, input);
}

void gxHWShader::sendAttrib3fv(const char* name, const float* input)
{
	int loc=getAttribLoc(name);
	glVertexAttrib3fv(loc, input);
}

void gxHWShader::sendAttrib4fv(const char* name, const float* input)
{
	int loc=getAttribLoc(name);
	glVertexAttrib4fv(loc, input);
}

void gxHWShader::sendAttrib1f(const char* name, float x)
{
	int loc=getAttribLoc(name);
	glVertexAttrib1f(loc, x);
}
