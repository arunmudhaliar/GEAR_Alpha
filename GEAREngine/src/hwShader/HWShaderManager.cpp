#include "HWShaderManager.h"

HWShaderManager::HWShaderManager()
{
}

HWShaderManager::~HWShaderManager()
{
	Reset();
}

void HWShaderManager::Init()
{
#if defined (USE_ProgrammablePipeLine)
        LoadDefaultShaders();
#endif
}

void HWShaderManager::Reset()
{
#if defined (USE_ProgrammablePipeLine)
	for(std::vector<gxHWShader*>::iterator it = m_cvHWShaderLst.begin(); it != m_cvHWShaderLst.end(); ++it)
	{
		gxHWShader* shader = *it;
		GX_DELETE(shader);
	}
	m_cvHWShaderLst.clear();

	std::vector<stHWShaderSnippet*>* submaplist=&m_cvHWShaderSnippets;
	for(std::vector<stHWShaderSnippet*>::iterator it = submaplist->begin(); it != submaplist->end(); ++it)
	{
		stHWShaderSnippet* snippet = *it;
		GX_DELETE(snippet);
	}
	m_cvHWShaderSnippets.clear();

	for(std::vector<gxSurfaceShader*>::iterator it = m_cvHWSurfaceShader.begin(); it != m_cvHWSurfaceShader.end(); ++it)
	{
		gxSurfaceShader* surfaceshader = *it;
		GX_DELETE(surfaceshader);
	}
	m_cvHWSurfaceShader.clear();
#endif
}

gxSurfaceShader* HWShaderManager::LoadSurfaceShader(const char* filename)
{
	for(std::vector<gxSurfaceShader*>::iterator it = m_cvHWSurfaceShader.begin(); it != m_cvHWSurfaceShader.end(); ++it)
	{
		gxSurfaceShader* surfaceshader = *it;
		if(strcmp(gxUtil::getFileNameFromPath(filename), surfaceshader->getSurfaceShaderFileName())==0)
			return surfaceshader;
	}

	gxSurfaceShader* newShader = new gxSurfaceShader();
	if(!newShader->loadSurfaceShader(filename))
	{
		GX_DELETE(newShader);
		return NULL;
	}

	m_cvHWSurfaceShader.push_back(newShader);
	return newShader;
}

void HWShaderManager::LoadDefaultShaders()
{
#if defined (USE_ProgrammablePipeLine)
#if defined(WIN32)

	//load code snippets
	stHWShaderSnippet* snippet=LoadCodeSnippet("res/shadersWin32/snippets/matrices_uniform_vars.snippet");
	if(snippet)
		m_cvHWShaderSnippets.push_back(snippet);
	snippet=LoadCodeSnippet("res/shadersWin32/snippets/vertex_attrib_vars.snippet");
	if(snippet)
		m_cvHWShaderSnippets.push_back(snippet);
	snippet=LoadCodeSnippet("res/shadersWin32/snippets/vertex_main.snippet");
	if(snippet)
		m_cvHWShaderSnippets.push_back(snippet);
	snippet=LoadCodeSnippet("res/shadersWin32/snippets/fragment_main.snippet");
	if(snippet)
		m_cvHWShaderSnippets.push_back(snippet);
	snippet=LoadCodeSnippet("res/shadersWin32/snippets/light_unifrom_vars.snippet");
	if(snippet)
		m_cvHWShaderSnippets.push_back(snippet);
	snippet=LoadCodeSnippet("res/shadersWin32/snippets/material_unifrom_vars.snippet");
	if(snippet)
		m_cvHWShaderSnippets.push_back(snippet);
	snippet=LoadCodeSnippet("res/shadersWin32/snippets/time_uniform_vars.snippet");
	if(snippet)
		m_cvHWShaderSnippets.push_back(snippet);
	snippet=LoadCodeSnippet("res/shadersWin32/snippets/fog_vars.snippet");
	if(snippet)
		m_cvHWShaderSnippets.push_back(snippet);

	//HW shaders
	gxHWShader* pShader=new gxHWShader();
    if(pShader->loadShader("res/shadersWin32/hwshader/only_diffuse.glsl"))	//0
		m_cvHWShaderLst.push_back(pShader);
	else
		GX_DELETE(pShader);
	pShader=new gxHWShader();
    if(pShader->loadShader("res/shadersWin32/hwshader/only_diffuse_with_color_pointer.glsl"))	//1
		m_cvHWShaderLst.push_back(pShader);
	else
		GX_DELETE(pShader);
	pShader=new gxHWShader();
    if(pShader->loadShader("res/shadersWin32/hwshader/guishader.glsl"))	//2
		m_cvHWShaderLst.push_back(pShader);
	else
		GX_DELETE(pShader);
	pShader=new gxHWShader();
    if(pShader->loadShader("res/shadersWin32/hwshader/blurshader.glsl"))	//3
		m_cvHWShaderLst.push_back(pShader);
	else
		GX_DELETE(pShader);
	pShader=new gxHWShader();
    if(pShader->loadShader("res/shadersWin32/hwshader/shadowmap.glsl"))	//4
		m_cvHWShaderLst.push_back(pShader);
	else
		GX_DELETE(pShader);

	//surface shaders
	LoadSurfaceShader("res//shadersWin32//surfaceShader//Diffuse.shader");
	LoadSurfaceShader("res//shadersWin32//surfaceShader//Diffuse_vlit.shader");
	LoadSurfaceShader("res//shadersWin32//surfaceShader//NormalMap.shader");
	LoadSurfaceShader("res//shadersWin32//surfaceShader//NormalMapSpecular.shader");
	LoadSurfaceShader("res//shadersWin32//surfaceShader//MultiTex.shader");
	LoadSurfaceShader("res//shadersWin32//surfaceShader//NormalMapSeperateSpecular.shader");

#elif defined(ANDROID)
		//load code snippets
	stHWShaderSnippet* snippet=LoadCodeSnippet("/storage/emulated/0/gear/shadersAndroid/snippets/matrices_uniform_vars.snippet");
	if(snippet)
		m_cvHWShaderSnippets.push_back(snippet);
	snippet=LoadCodeSnippet("/storage/emulated/0/gear/shadersAndroid/snippets/vertex_attrib_vars.snippet");
	if(snippet)
		m_cvHWShaderSnippets.push_back(snippet);
	snippet=LoadCodeSnippet("/storage/emulated/0/gear/shadersAndroid/snippets/vertex_main.snippet");
	if(snippet)
		m_cvHWShaderSnippets.push_back(snippet);
	snippet=LoadCodeSnippet("/storage/emulated/0/gear/shadersAndroid/snippets/fragment_main.snippet");
	if(snippet)
		m_cvHWShaderSnippets.push_back(snippet);
	snippet=LoadCodeSnippet("/storage/emulated/0/gear/shadersAndroid/snippets/light_unifrom_vars.snippet");
	if(snippet)
		m_cvHWShaderSnippets.push_back(snippet);
	snippet=LoadCodeSnippet("/storage/emulated/0/gear/shadersAndroid/snippets/material_unifrom_vars.snippet");
	if(snippet)
		m_cvHWShaderSnippets.push_back(snippet);
	snippet=LoadCodeSnippet("/storage/emulated/0/gear/shadersAndroid/snippets/time_uniform_vars.snippet");
	if(snippet)
		m_cvHWShaderSnippets.push_back(snippet);

	//HW shaders
	gxHWShader* pShader=new gxHWShader();
    if(pShader->loadShader("/storage/emulated/0/gear/shadersAndroid/hwshader/only_diffuse.glsl"))	//0
		m_cvHWShaderLst.push_back(pShader);
	else
		GX_DELETE(pShader);
	pShader=new gxHWShader();
    if(pShader->loadShader("/storage/emulated/0/gear/shadersAndroid/hwshader/only_diffuse_with_color_pointer.glsl"))	//1
		m_cvHWShaderLst.push_back(pShader);
	else
		GX_DELETE(pShader);
	pShader=new gxHWShader();
    if(pShader->loadShader("/storage/emulated/0/gear/shadersAndroid/hwshader/guishader.glsl"))	//2
		m_cvHWShaderLst.push_back(pShader);
	else
		GX_DELETE(pShader);
	pShader=new gxHWShader();
    if(pShader->loadShader("/storage/emulated/0/gear/shadersAndroid/hwshader/blurshader.glsl"))	//3
		m_cvHWShaderLst.push_back(pShader);
	else
		GX_DELETE(pShader);
	pShader=new gxHWShader();
    if(pShader->loadShader("/storage/emulated/0/gear/shadersAndroid/hwshader/shadowmap.glsl"))	//4
		m_cvHWShaderLst.push_back(pShader);
	else
		GX_DELETE(pShader);

	//surface shaders
	LoadSurfaceShader("//storage//emulated//0//gear//shadersAndroid//surfaceShader//Diffuse.shader");
	LoadSurfaceShader("//storage//emulated//0//gear//shadersAndroid//surfaceShader//Diffuse_vlit.shader");
	LoadSurfaceShader("//storage//emulated//0//gear//shadersAndroid//surfaceShader//NormalMap.shader");
	LoadSurfaceShader("//storage//emulated//0//gear//shadersAndroid//surfaceShader//NormalMapSpecular.shader");
	LoadSurfaceShader("//storage//emulated//0//gear//shadersAndroid//surfaceShader//NormalMapSeperateSpecular.shader");
#endif
#endif
}

gxHWShader* HWShaderManager::LoadShaderFromBuffer(const char* name, const char* buffer, int size)
{
	for(std::vector<gxHWShader*>::iterator it = m_cvHWShaderLst.begin(); it != m_cvHWShaderLst.end(); ++it)
	{
		gxHWShader* hwshaderNode = *it;
		if(strcmp(hwshaderNode->getShaderName(), name)==0)
			return hwshaderNode;
	}

	if(buffer==NULL || size==0) return NULL;

	gxHWShader* shader = new gxHWShader();
	if(!shader->loadShaderFromBuffer(name, buffer, size))
	{
		GX_DELETE(shader);
	}
	m_cvHWShaderLst.push_back(shader);
	return shader;
}

HWShaderManager::stHWShaderSnippet* HWShaderManager::LoadCodeSnippet(const char* filename)
{
	//read snippet code
	int fileSz=0;
	FILE* fp=fopen(filename, "r");
	if(fp==NULL) return NULL;

	fseek(fp, 0, SEEK_END);
	fileSz=ftell(fp);
	fclose(fp);
	
	if(!fileSz) return NULL;
	
	//vertex shader source
	fp=fopen(filename, "r");
	if(fp==NULL) return NULL;

	stHWShaderSnippet* newSnippetCode = new stHWShaderSnippet();

	newSnippetCode->size=fileSz;
	newSnippetCode->snippet=new char[newSnippetCode->size+1];
	memset((void*)newSnippetCode->snippet, 0, newSnippetCode->size);
	fread((void*)newSnippetCode->snippet, 1, newSnippetCode->size, fp);
	fclose(fp);
	newSnippetCode->snippet[newSnippetCode->size]='\0';
	//

#if DEBUG
	DEBUG_PRINT("SNIPPET:\n%s", newSnippetCode->snippet);
	DEBUG_PRINT("%s loaded", filename);
#endif

	return newSnippetCode;
}

#if defined (USE_ProgrammablePipeLine)
gxHWShader* HWShaderManager::GetHWShader(int index)
{
	if(index>=(int)m_cvHWShaderLst.size()) return NULL;
	return m_cvHWShaderLst[index];
}
#endif

void HWShaderManager::update(float dt)
{
//#if defined (USE_ProgrammablePipeLine)
//	for(std::vector<gxHWShader*>::iterator it = m_cvHWShaderLst.begin(); it != m_cvHWShaderLst.end(); ++it)
//	{
//		gxHWShader* hwshaderNode = *it;
//		hwshaderNode->updateShaderVars(dt);
//	}
//#endif
}
