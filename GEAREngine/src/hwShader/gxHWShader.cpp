#include "gxHWShader.h"
//#include "../util/objC_util.h"

gxHWShader::gxHWShader()
{
	shaderProgram=0;
	vertexShaderID=0;
	fragmentShaderID=0;

	//predefined vars
	unifrom_GEAR_MODELVIEW=-1;
	unifrom_GEAR_MVP=-1;
	unifrom_GEAR_MODEL_MATRIX=-1;
	unifrom_GEAR_MODEL_INVERSE=-1;
	unifrom_material_diffuse=-1;
	unifrom_material_ambient=-1;
	unifrom_material_specular=-1;
	unifrom_material_shininess=-1;
	unifrom_GEAR_Time=-1;
	unifrom_GEAR_ScreenParams=-1;

	attrib_vIN_Position=-1;
	attrib_vIN_Normal=-1;
	attrib_Tangent=-1;
	//

}

gxHWShader::~gxHWShader()
{
	clearAttribRefVarList();
	clearUniformRefVarList();
}

bool gxHWShader::loadShaderFromBuffer(const std::string& name, const std::string& shaderBuffer)
{
	const char* define_vertex="#define GEAR_VERTEX_SHADER\n";
	const char* define_fragment="#define GEAR_FRAGMENT_SHADER\n";

	int vSz=(int)shaderBuffer.length()+(int)strlen(define_vertex);
	char* vsource=new char[vSz];
	memset((void*)vsource, 0, vSz);
	strcpy(vsource, define_vertex);
	strncpy(&vsource[strlen(define_vertex)], shaderBuffer.c_str(), shaderBuffer.length());

	int fSz=(int)shaderBuffer.size()+(int)strlen(define_fragment);
	char* fsource=new char[fSz];
	memset((void*)fsource, 0, fSz);
	strcpy(fsource, define_fragment);
	strncpy(&fsource[strlen(define_fragment)], shaderBuffer.c_str(), shaderBuffer.length());

	// Create and compile vertex shader
    std::string vbuffer;
    vbuffer.assign(vsource, vSz);
	if (!compileShader(&vertexShaderID, GL_VERTEX_SHADER, vbuffer))
	{
		DEBUG_PRINT("Failed to compile vertex shader from buffer");
		GX_DELETE_ARY(vsource);
		GX_DELETE_ARY(fsource);
		return false;
	}
	
	// Create and compile fragment shader
    std::string fbuffer;
    fbuffer.assign(fsource, fSz);
	if (!compileShader(&fragmentShaderID, GL_FRAGMENT_SHADER, fbuffer))
	{
		DEBUG_PRINT("Failed to compile fragment shader from buffer");
		GX_DELETE_ARY(vsource);
		GX_DELETE_ARY(fsource);
		return false;
	}
	
	GX_DELETE_ARY(vsource);
	GX_DELETE_ARY(fsource);

	// Create shader program
	shaderProgram = glCreateProgram();

	attachShader();
	
	// Link program
	if (!linkProgram())
	{
		DEBUG_PRINT("Failed to link program: %d from buffer", shaderProgram);
		destroyShader();
		GX_DELETE_ARY(vsource);
		GX_DELETE_ARY(fsource);
		return false;
	}
	
	validateProgram();
    detachShader();
	disableProgram();
	
	shaderName.assign(name);

	return true;
}

bool gxHWShader::loadShader(const std::string& shaderFile)
{
	DEBUG_PRINT("Load shader %s", shaderFile.c_str());

	//read shader code
	int fileSz=0;
	FILE* fp=fopen(shaderFile.c_str(), "r");
	if(fp==NULL)
	{
		DEBUG_PRINT("Shader %s FILE open ERROR #1", shaderFile.c_str());
		return false;
	}

	fseek(fp, 0, SEEK_END);
	fileSz=(int)ftell(fp);
	fclose(fp);
	
	if(!fileSz)
	{
		DEBUG_PRINT("Shader %s FILE size ZERO ERROR", shaderFile.c_str());
		return false;
	}
	
#ifdef ANDROID
	const char* define_vertex="#define GEAR_VERTEX_SHADER\n";
#else
	const char* define_vertex="#version 120\n#define GEAR_VERTEX_SHADER\n";
#endif
	const char* define_fragment="#define GEAR_FRAGMENT_SHADER\n";

	//vertex shader source
	fp=fopen(shaderFile.c_str(), "r");
	if(fp==NULL)
	{
		DEBUG_PRINT("Shader %s FILE open ERROR #2", shaderFile.c_str());
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
	fp=fopen(shaderFile.c_str(), "r");
	if(fp==NULL)
	{
		DEBUG_PRINT("Shader %s FILE open ERROR #3", shaderFile.c_str());
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
    std::string vbuffer;
    vbuffer.assign(vsource, vSz);
	if (!compileShader(&vertexShaderID, GL_VERTEX_SHADER, vbuffer))
	{
		DEBUG_PRINT("Failed to compile vertex shader '%s'", shaderFile.c_str());
		GX_DELETE_ARY(vsource);
		GX_DELETE_ARY(fsource);
		return false;
	}
	
	// Create and compile fragment shader
    std::string fbuffer;
    fbuffer.assign(fsource, fSz);
	if (!compileShader(&fragmentShaderID, GL_FRAGMENT_SHADER, fbuffer))
	{
		DEBUG_PRINT("Failed to compile fragment shader '%s'", shaderFile.c_str());
		GX_DELETE_ARY(vsource);
		GX_DELETE_ARY(fsource);
		return false;
	}
	
	GX_DELETE_ARY(vsource);
	GX_DELETE_ARY(fsource);

	// Create shader program
	shaderProgram = glCreateProgram();

	attachShader();
	
	// Link program
	if (!linkProgram())
	{
		DEBUG_PRINT("Failed to link program: %d (shader : %s)", shaderProgram, shaderFile.c_str());
		destroyShader();
		GX_DELETE_ARY(vsource);
		GX_DELETE_ARY(fsource);
		return false;
	}
	
	validateProgram();
    detachShader();
	disableProgram();
	
	shaderName.assign(shaderFile);

	DEBUG_PRINT("Shader Loaded %s", shaderFile.c_str());

	return true;
}

bool gxHWShader::compileShader(GLuint* shader, GLenum type, const std::string& source)
{
    GLint status;
    const char* buffer = source.c_str();
    int bufferSz = (int)source.length();
    
    *shader = glCreateShader(type);
    CHECK_GL_ERROR(glShaderSource(*shader, 1, &buffer, &bufferSz));
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
	
    CHECK_GL_ERROR(glLinkProgram(shaderProgram));
	
#if defined(DEBUG) || defined(_DEBUG)	//DEBUG for MAC & _DEBUG for Win32
    GLint logLength;
    glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 1)
    {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetProgramInfoLog(shaderProgram, logLength, &logLength, log);
        DEBUG_PRINT("Program link log:\n%s", log);
        free(log);
    }
#endif
	
    CHECK_GL_ERROR(glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status));
    if (status == 0)
        return false;
	
    return true;
}

void gxHWShader::attachShader()
{
	// Attach vertex shader to program
	CHECK_GL_ERROR(glAttachShader(shaderProgram, vertexShaderID));
	// Attach fragment shader to program
	CHECK_GL_ERROR(glAttachShader(shaderProgram, fragmentShaderID));	
}

void gxHWShader::detachShader()
{
	// Release vertex and fragment shaders
	if (vertexShaderID)
		CHECK_GL_ERROR(glDetachShader(shaderProgram, vertexShaderID));
	if (fragmentShaderID)
		CHECK_GL_ERROR(glDetachShader(shaderProgram, fragmentShaderID));

	clearAttribRefVarList();
	clearUniformRefVarList();
}

void gxHWShader::destroyShader()
{
	detachShader();

	if (vertexShaderID)
	{
		CHECK_GL_ERROR(glDeleteShader(vertexShaderID));
		vertexShaderID = 0;
	}
	if (fragmentShaderID)
	{
		CHECK_GL_ERROR(glDeleteShader(fragmentShaderID));
		fragmentShaderID = 0;
	}
	if (shaderProgram)
	{
		CHECK_GL_ERROR(glDeleteProgram(shaderProgram));
		shaderProgram = 0;
	}	
}

void gxHWShader::enableProgram()
{
	// Use shader program
    CHECK_GL_ERROR(glUseProgram(shaderProgram));
}

void gxHWShader::disableProgram()
{
	CHECK_GL_ERROR(glUseProgram(0));
}

bool gxHWShader::validateProgram()
{
	GLint logLength, status;
	
	CHECK_GL_ERROR(glValidateProgram(shaderProgram));

#if defined(DEBUG) || defined(_DEBUG)	//DEBUG for MAC & _DEBUG for Win32
	glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 1)
	{
		GLchar *log = (GLchar *)malloc(logLength);
		glGetProgramInfoLog(shaderProgram, logLength, &logLength, log);
		DEBUG_PRINT("Program validate log:\n%s", log);
		free(log);
	}
#endif

	CHECK_GL_ERROR(glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &status));
	if (status == 0)
		return false;
	
	return true;
}

void gxHWShader::showShaderLog(const std::string& title)
{
	GLint logLength;
	CHECK_GL_ERROR(glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &logLength));
	if (logLength > 1)
	{
		GLchar *log = (GLchar *)malloc(logLength);
		CHECK_GL_ERROR(glGetProgramInfoLog(shaderProgram, logLength, &logLength, log));
		DEBUG_PRINT("title :%s\n%s", title.c_str(), log);
		free(log);
	}
}

int gxHWShader::getAttribLoc(const char* avar)
{
	for(std::vector<stAttribLocation*>::iterator it = attribRefVariableList.begin(); it != attribRefVariableList.end(); ++it)
	{
		stAttribLocation* attribRefVar = *it;
		if(strcmp(attribRefVar->attrib_ref_name, avar)==0)
		{
			return attribRefVar->attrib_ref;
		}
	}

	int loc = glGetAttribLocation(shaderProgram, avar);
	if(loc == -1)
	{
		DEBUG_PRINT("getAttribLoc() returned -1 for %s", avar);
		return -1;
	}

	stAttribLocation* newAttribRefVar=new stAttribLocation();
	GX_STRCPY(newAttribRefVar->attrib_ref_name, avar);
	newAttribRefVar->attrib_ref=loc;

	attribRefVariableList.push_back(newAttribRefVar);

	return loc;
}

void gxHWShader::clearAttribRefVarList()
{
	for(std::vector<stAttribLocation*>::iterator it = attribRefVariableList.begin(); it != attribRefVariableList.end(); ++it)
	{
		stAttribLocation* attribRefVar = *it;
		GX_DELETE(attribRefVar);
	}

	attribRefVariableList.clear();
}

int gxHWShader::getUniformLoc(const char* uvar)
{
	for(std::vector<stUniformLocation*>::iterator it = uniformRefVariableList.begin(); it != uniformRefVariableList.end(); ++it)
	{
		stUniformLocation* uniformRefVarnode = *it;
		if(strcmp(uniformRefVarnode->uniform_ref_name, uvar)==0)
		{
			return uniformRefVarnode->uniform_ref;
		}
	}

	int loc = glGetUniformLocation(shaderProgram, uvar);
	if(loc == -1)
	{
		DEBUG_PRINT("getUniformLoc() returned -1 for %s", uvar);
		return -1;
	}

	stUniformLocation* newUnifromRefVar=new stUniformLocation();
	GX_STRCPY(newUnifromRefVar->uniform_ref_name, uvar);
	newUnifromRefVar->uniform_ref=loc;

	uniformRefVariableList.push_back(newUnifromRefVar);

	return loc;
}

void gxHWShader::clearUniformRefVarList()
{
	for(std::vector<stUniformLocation*>::iterator it = uniformRefVariableList.begin(); it != uniformRefVariableList.end(); ++it)
	{
		stUniformLocation* uniformRefVarnode = *it;
		GX_DELETE(uniformRefVarnode);
	}

	uniformRefVariableList.clear();
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
	if(unifrom_GEAR_MODELVIEW==-1)
	{
		unifrom_GEAR_MODELVIEW = getUniformLoc("GEAR_MODELVIEW");
	}
	
	CHECK_GL_ERROR(glUniformMatrix4fv(unifrom_GEAR_MODELVIEW, 1, false, input));
}

void gxHWShader::sendUniform_GEAR_MVP(const float* input)
{
	if(unifrom_GEAR_MVP==-1)
	{
		unifrom_GEAR_MVP = getUniformLoc("GEAR_MVP");
	}

	CHECK_GL_ERROR(glUniformMatrix4fv(unifrom_GEAR_MVP, 1, false, input));
}

void gxHWShader::sendUniform_GEAR_MODEL_MATRIX(const float* input)
{
	if(unifrom_GEAR_MODEL_MATRIX==-1)
	{
		unifrom_GEAR_MODEL_MATRIX = getUniformLoc("GEAR_MODEL_MATRIX");
	}
	
	CHECK_GL_ERROR(glUniformMatrix4fv(unifrom_GEAR_MODEL_MATRIX, 1, false, input));
}

void gxHWShader::sendUniform_GEAR_MODEL_INVERSE(const float* input)
{
	if(unifrom_GEAR_MODEL_INVERSE==-1)
	{
		unifrom_GEAR_MODEL_INVERSE = getUniformLoc("GEAR_MODEL_INVERSE");
	}
		
	CHECK_GL_ERROR(glUniformMatrix4fv(unifrom_GEAR_MODEL_INVERSE, 1, false, input));
}

void gxHWShader::sendUniform_material_diffuse(const float* input)
{
	if(unifrom_material_diffuse==-1)
	{
		unifrom_material_diffuse = getUniformLoc("material.diffuse");
	}

	CHECK_GL_ERROR(glUniform4fv(unifrom_material_diffuse, 1, input));
}

void gxHWShader::sendUniform_material_ambient(const float* input)
{
	if(unifrom_material_ambient==-1)
	{
		unifrom_material_ambient = getUniformLoc("material.ambient");
	}
	
	CHECK_GL_ERROR(glUniform4fv(unifrom_material_ambient, 1, input));
}

void gxHWShader::sendUniform_material_specular(const float* input)
{
	if(unifrom_material_specular==-1)
	{
		unifrom_material_specular = getUniformLoc("material.specular");
	}
	
	CHECK_GL_ERROR(glUniform4fv(unifrom_material_specular, 1, input));
}

void gxHWShader::sendUniform_material_shininess(float input)
{
	if(unifrom_material_shininess==-1)
	{
		unifrom_material_shininess = getUniformLoc("material.shininess");
	}

	CHECK_GL_ERROR(glUniform1f(unifrom_material_shininess, input));
}

void gxHWShader::sendUniform_GEAR_Time(const float* time)
{
	if(unifrom_GEAR_Time==-1)
	{
		unifrom_GEAR_Time = getUniformLoc("GEAR_Time");
	}
	
	CHECK_GL_ERROR(glUniform4fv(unifrom_GEAR_Time, 1, time));
}

void gxHWShader::sendUniform_GEAR_ScreenParams(const float* screenParams)
{
	if(unifrom_GEAR_ScreenParams==-1)
	{
		unifrom_GEAR_ScreenParams = getUniformLoc("GEAR_ScreenParams");
	}

	CHECK_GL_ERROR(glUniform4fv(unifrom_GEAR_ScreenParams, 1, screenParams));
}

int gxHWShader::getAttrib_vIN_Position()
{
	if(attrib_vIN_Position==-1)
	{
		attrib_vIN_Position=getAttribLoc("vIN_Position");
	}
	return attrib_vIN_Position;
}

int gxHWShader::getAttrib_vIN_Normal()
{
	if(attrib_vIN_Normal==-1)
	{
		attrib_vIN_Normal=getAttribLoc("vIN_Normal");
	}
	return attrib_vIN_Normal;
}

int gxHWShader::getAttrib_Tangent()
{
	if(attrib_Tangent==-1)
	{
		attrib_Tangent=getAttribLoc("Tangent");
	}
	return attrib_Tangent;
}