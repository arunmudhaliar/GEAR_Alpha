#ifndef HWSHADERMANAGER_H
#define HWSHADERMANAGER_H

#include "../renderer/gxRenderer.h"
//#include "gxShader.h"


#if defined (USE_ProgrammablePipeLine)
//#include "gxHWShader.h"
#include "gxOnlyDiffuse.h"
#include "gxDiffuseUnlit.h"
#include "gxOnlyDiffuseWithColor.h"
#include "gxGenericShader.h"
#include <vector>
#endif

class __declspec( dllexport ) HWShaderManager
{
public:
	HWShaderManager();
	~HWShaderManager();
	
	void Init();
	void Reset();
    
    void update(float dt);
	
#if defined (USE_ProgrammablePipeLine)
	gxHWShader* GetHWShader(int index);
#endif
    
private:
	void LoadDefaultShaders();
	
#if defined (USE_ProgrammablePipeLine)
	//default shaders
	gxOnlyDiffuse m_cOnlyDiffuse;
	gxDiffuseUnlit m_cDiffuseUnlit;
	gxOnlyDiffuseWithColor m_cOnlyDiffuseWithColor;
	gxGenericShader m_cGenericShader;
	gxGenericShader m_cLightingOnlyGenericShader;
	gxGenericShader m_cLightingOnlyFirstPassGenericShader;
	std::vector<gxHWShader*> m_cvHWShaderLst;
#endif
};

#endif
