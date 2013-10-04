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

bool gxHWShader::loadShader(const char* vShaderFile, const char* fShaderFile)
{
	const char* vertShaderPathname=NULL;
	const char* fragShaderPathname=NULL;
	
	//FILE* fp=fopen(vShaderFile, "r");
	//int nScript=0;
	//fscanf(fp, "%d\n", &nScript);
	//fclose(fp);

	// Create shader program
	m_cProgram = glCreateProgram();
	
	// Create and compile vertex shader
	
	vertShaderPathname = vShaderFile;//getResourcePath(vShaderFile);
	if (!compileShader(&m_cVertShader, GL_VERTEX_SHADER, vertShaderPathname))
	{
		DEBUG_PRINT("Failed to compile vertex shader");
		return false;
	}
	
	// Create and compile fragment shader
	fragShaderPathname = fShaderFile;//getResourcePath(fShaderFile);
	if (!compileShader(&m_cFragShader, GL_FRAGMENT_SHADER, fragShaderPathname))
	{
		DEBUG_PRINT("Failed to compile fragment shader");
		return false;
	}
	
	attachShader();
	bind();
	
	// Link program
	if (!linkProgram())
	{
		DEBUG_PRINT("Failed to link program: %d", m_cProgram);
		destroyShader();
		return false;
	}
	
	getUniformVars();
	
	validateProgram();

    detachShader();
    
	disableProgram();
		
	return true;
}

bool gxHWShader::compileShader(GLuint* shader, GLenum type, const char* file)
{
    GLint status;
    const GLchar *source=NULL;
	
	int fileSz=0;
	FILE* fp=fopen(file, "r");
	fseek(fp, 0, SEEK_END);
	fileSz=ftell(fp);
	fclose(fp);
	
	if(!fileSz) return false;
	
	source=new char[fileSz];
	memset((void*)source, 0, fileSz);
	fp=fopen(file, "r");
	fread((void*)source, 1, fileSz, fp);
	fclose(fp);
	
    *shader = glCreateShader(type);
    glShaderSource(*shader, 1, &source, &fileSz);
    glCompileShader(*shader);
	
#if defined(DEBUG) || defined(_DEBUG)	//DEBUG for MAC & _DEBUG for Win32
	GLint logLength;
    glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0)
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
		GX_DELETE_ARY(source);
        return false;
    }
	
	GX_DELETE_ARY(source);
    return true;
}

bool gxHWShader::linkProgram()
{
    GLint status;
	
    glLinkProgram(m_cProgram);
	
#if defined(DEBUG) || defined(_DEBUG)	//DEBUG for MAC & _DEBUG for Win32
    GLint logLength;
    glGetProgramiv(m_cProgram, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0)
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
	if (logLength > 0)
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
	if (logLength > 0)
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
