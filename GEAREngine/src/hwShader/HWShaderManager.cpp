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
#endif
}

void HWShaderManager::LoadDefaultShaders()
{
#if defined (USE_ProgrammablePipeLine)
#if defined(WIN32)
    if(m_cOnlyDiffuse.loadShader("res/shadersWin32/hwshader/only_diffuse.shader"))
		m_cvHWShaderLst.push_back(&m_cOnlyDiffuse);
    if(m_cDiffuseUnlit.loadShader("res/shadersWin32/hwshader/diffusemapunlit.shader"))
		m_cvHWShaderLst.push_back(&m_cDiffuseUnlit);
    if(m_cOnlyDiffuseWithColor.loadShader("res/shadersWin32/hwshader/only_diffuse_with_color_pointer.shader"))
		m_cvHWShaderLst.push_back(&m_cOnlyDiffuseWithColor);
    if(m_cGenericShader.loadShader("res/shadersWin32/hwshader/pvLightingShader.shader"))
		m_cvHWShaderLst.push_back(&m_cGenericShader);
    if(m_cLightingOnlyFirstPassGenericShader.loadShader("res/shadersWin32/hwshader/pvLightingOnlyShaderFirstPass.shader"))
		m_cvHWShaderLst.push_back(&m_cLightingOnlyFirstPassGenericShader);
	if(m_cLightingOnlyGenericShader.loadShader("res/shadersWin32/hwshader/pvLightingOnlyShader.shader"))
		m_cvHWShaderLst.push_back(&m_cLightingOnlyGenericShader);
	if(m_cSpriteGenericShader.loadShader("res/shadersWin32/hwshader/guishader.shader"))
		m_cvHWShaderLst.push_back(&m_cSpriteGenericShader);
	if(m_cBlurGenericShader.loadShader("res/shadersWin32/hwshader/blurshader.shader"))
		m_cvHWShaderLst.push_back(&m_cBlurGenericShader);
#endif
#endif
}

#if defined (USE_ProgrammablePipeLine)
gxHWShader* HWShaderManager::GetHWShader(int index)
{
	if(index>=m_cvHWShaderLst.size()) return NULL;
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
