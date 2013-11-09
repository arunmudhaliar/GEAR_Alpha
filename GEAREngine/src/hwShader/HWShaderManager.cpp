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
	m_cvHWShaderLst.clear();

	std::vector<stHWShaderSnippet*>* submaplist=&m_cvHWShaderSnippets;
	for(std::vector<stHWShaderSnippet*>::iterator it = submaplist->begin(); it != submaplist->end(); ++it)
	{
		stHWShaderSnippet* snippet = *it;
		GX_DELETE(snippet);
	}
	m_cvHWShaderSnippets.clear();
#endif
}

void HWShaderManager::LoadDefaultShaders()
{
#if defined (USE_ProgrammablePipeLine)
#if defined(WIN32)

	//load code snippets
	stHWShaderSnippet* snippet=LoadCodeSnippet("res/shadersWin32/snippets/matrices_uniform_vars.snippet");
	if(snippet)
		m_cvHWShaderSnippets.push_back(snippet);
	snippet=LoadCodeSnippet("res/shadersWin32/snippets/vertex_inout_vars.snippet");
	if(snippet)
		m_cvHWShaderSnippets.push_back(snippet);
	snippet=LoadCodeSnippet("res/shadersWin32/snippets/lightandmaterial_unifrom_vars.snippet");
	if(snippet)
		m_cvHWShaderSnippets.push_back(snippet);

    if(m_cOnlyDiffuse.loadShader("res/shadersWin32/hwshader/only_diffuse.glsl"))	//0
		m_cvHWShaderLst.push_back(&m_cOnlyDiffuse);
    if(m_cDiffuseUnlit.loadShader("res/shadersWin32/hwshader/diffusemapunlit.glsl"))	//1
		m_cvHWShaderLst.push_back(&m_cDiffuseUnlit);
    if(m_cOnlyDiffuseWithColor.loadShader("res/shadersWin32/hwshader/only_diffuse_with_color_pointer.glsl"))	//2
		m_cvHWShaderLst.push_back(&m_cOnlyDiffuseWithColor);
    if(m_cLightingOnlyFirstPassGenericShader.loadShader("res/shadersWin32/hwshader/pvLightingOnlyShaderFirstPass.glsl"))	//3
		m_cvHWShaderLst.push_back(&m_cLightingOnlyFirstPassGenericShader);
	if(m_cSpriteGenericShader.loadShader("res/shadersWin32/hwshader/guishader.glsl"))	//4
		m_cvHWShaderLst.push_back(&m_cSpriteGenericShader);
	if(m_cBlurGenericShader.loadShader("res/shadersWin32/hwshader/blurshader.glsl"))	//5
		m_cvHWShaderLst.push_back(&m_cBlurGenericShader);
#endif
#endif
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
	newSnippetCode->snippet=new char[newSnippetCode->size];
	memset((void*)newSnippetCode->snippet, 0, newSnippetCode->size);
	fread((void*)newSnippetCode->snippet, 1, newSnippetCode->size, fp);
	fclose(fp);
	//

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
