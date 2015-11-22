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
#else
#error "Not implemented"
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
#else
#error "Not implemented"
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

	std::string resource_dir_root_path;

#if defined(WIN32)
	resource_dir_root_path="res/shadersWin32/";
#elif defined(ANDROID)
	resource_dir_root_path="/storage/emulated/0/gear/shadersAndroid/";
#elif defined(__APPLE__)
    resource_dir_root_path="res/shadersWin32/";
#else
	DEBUG_PRINT("ERROR : Shaders not implemented for this platform ");
	return;
#endif

	//load code snippets
	stHWShaderSnippet* snippet=LoadCodeSnippet((resource_dir_root_path+"snippets/matrices_uniform_vars.snippet").c_str());
	if(snippet)
		m_cvHWShaderSnippets.push_back(snippet);
	snippet=LoadCodeSnippet((resource_dir_root_path+"snippets/vertex_attrib_vars.snippet").c_str());
	if(snippet)
		m_cvHWShaderSnippets.push_back(snippet);
	snippet=LoadCodeSnippet((resource_dir_root_path+"snippets/vertex_main.snippet").c_str());
	if(snippet)
		m_cvHWShaderSnippets.push_back(snippet);
	snippet=LoadCodeSnippet((resource_dir_root_path+"snippets/fragment_main.snippet").c_str());
	if(snippet)
		m_cvHWShaderSnippets.push_back(snippet);
	snippet=LoadCodeSnippet((resource_dir_root_path+"snippets/light_unifrom_vars.snippet").c_str());
	if(snippet)
		m_cvHWShaderSnippets.push_back(snippet);
	snippet=LoadCodeSnippet((resource_dir_root_path+"snippets/material_unifrom_vars.snippet").c_str());
	if(snippet)
		m_cvHWShaderSnippets.push_back(snippet);
	snippet = LoadCodeSnippet((resource_dir_root_path + "snippets/vertex_with_shadow_main.snippet").c_str());
	if (snippet)
		m_cvHWShaderSnippets.push_back(snippet);
	snippet = LoadCodeSnippet((resource_dir_root_path + "snippets/fragment_with_shadow_main.snippet").c_str());
	if (snippet)
		m_cvHWShaderSnippets.push_back(snippet);

	//HW shaders
	gxHWShader* pShader=new gxHWShader();
    if(pShader->loadShader((resource_dir_root_path+"hwshader/only_diffuse.glsl").c_str()))	//0
		m_cvHWShaderLst.push_back(pShader);
	else
		GX_DELETE(pShader);
	pShader=new gxHWShader();
    if(pShader->loadShader((resource_dir_root_path+"hwshader/only_diffuse_with_color_pointer.glsl").c_str()))	//1
		m_cvHWShaderLst.push_back(pShader);
	else
		GX_DELETE(pShader);
	pShader=new gxHWShader();
    if(pShader->loadShader((resource_dir_root_path+"hwshader/guishader.glsl").c_str()))	//2
		m_cvHWShaderLst.push_back(pShader);
	else
		GX_DELETE(pShader);
	pShader=new gxHWShader();
    if(pShader->loadShader((resource_dir_root_path+"hwshader/blurshader.glsl").c_str()))	//3
		m_cvHWShaderLst.push_back(pShader);
	else
		GX_DELETE(pShader);
	pShader=new gxHWShader();
    if(pShader->loadShader((resource_dir_root_path+"hwshader/shadowmap.glsl").c_str()))	//4
		m_cvHWShaderLst.push_back(pShader);
	else
		GX_DELETE(pShader);
	pShader=new gxHWShader();
    if(pShader->loadShader((resource_dir_root_path+"hwshader/fog_linear_shader.glsl").c_str()))	//5
		m_cvHWShaderLst.push_back(pShader);
	else
		GX_DELETE(pShader);
	pShader=new gxHWShader();
    if(pShader->loadShader((resource_dir_root_path+"hwshader/fog_exp_shader.glsl").c_str()))	//6
		m_cvHWShaderLst.push_back(pShader);
	else
		GX_DELETE(pShader);
	pShader=new gxHWShader();
    if(pShader->loadShader((resource_dir_root_path+"hwshader/fog_exp2_shader.glsl").c_str()))	//7
		m_cvHWShaderLst.push_back(pShader);
	else
		GX_DELETE(pShader);


	//surface shaders
	LoadSurfaceShader((resource_dir_root_path+"surfaceShader/Diffuse.shader").c_str());
	LoadSurfaceShader((resource_dir_root_path+"surfaceShader/Diffuse_vlit.shader").c_str());
	LoadSurfaceShader((resource_dir_root_path+"surfaceShader/NormalMap.shader").c_str());
	LoadSurfaceShader((resource_dir_root_path+"surfaceShader/NormalMapSpecular.shader").c_str());
	LoadSurfaceShader((resource_dir_root_path+"surfaceShader/MultiTex.shader").c_str());
	LoadSurfaceShader((resource_dir_root_path+"surfaceShader/NormalMapSeperateSpecular.shader").c_str());
	LoadSurfaceShader((resource_dir_root_path+"surfaceShader/NormalMapSeperateEmissive.shader").c_str());
	LoadSurfaceShader((resource_dir_root_path+"surfaceShader/NormalMapSeperateSpecularEmissive.shader").c_str());
#else
#error "Not implemented."
#endif
}

gxHWShader* HWShaderManager::LoadShaderFromFile(const char* relativePath)
{
	std::string resource_dir_root_path;

#if defined(WIN32)
	resource_dir_root_path = "res/shadersWin32/";
#elif defined(ANDROID)
	resource_dir_root_path = "/storage/emulated/0/gear/shadersAndroid/";
#elif defined(__APPLE__)
	resource_dir_root_path = "res/shadersWin32/";
#else
	DEBUG_PRINT("ERROR : Shaders not implemented for this platform ");
	return;
#endif

	//HW shaders
	gxHWShader* pShader = new gxHWShader();
	if (pShader->loadShader((resource_dir_root_path + relativePath).c_str()))
		m_cvHWShaderLst.push_back(pShader);
	else
		GX_DELETE(pShader);

	return pShader;
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
	long fileSz=0;
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
