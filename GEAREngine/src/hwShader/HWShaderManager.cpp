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
    if(m_cOnlyDiffuse.loadShader("res/shadersWin32/only_diffuse.vsh", "res/shadersWin32/only_diffuse.fsh"))
		m_cvHWShaderLst.push_back(&m_cOnlyDiffuse);
    if(m_cDiffuseUnlit.loadShader("res/shadersWin32/diffusemapunlit.vsh", "res/shadersWin32/diffusemapunlit.fsh"))
		m_cvHWShaderLst.push_back(&m_cDiffuseUnlit);
    if(m_cOnlyDiffuseWithColor.loadShader("res/shadersWin32/only_diffuse_with_color_pointer.vsh", "res/shadersWin32/only_diffuse_with_color_pointer.fsh"))
		m_cvHWShaderLst.push_back(&m_cOnlyDiffuseWithColor);
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
