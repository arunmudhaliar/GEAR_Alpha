#ifndef HWSHADERMANAGER_H
#define HWSHADERMANAGER_H

#include "../renderer/gxRenderer.h"
#if defined (USE_ProgrammablePipeLine)
#include "gxGenericShader.h"
#include <vector>
#endif

class __declspec( dllexport ) HWShaderManager
{
public:

	struct stHWShaderSnippet
	{
		stHWShaderSnippet()
		{
			snippet=NULL;
			size=0;
		}
		~stHWShaderSnippet()
		{
			GX_DELETE_ARY(snippet);
		}
		char* snippet;
		int size;
	};

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

private:
	stHWShaderSnippet* LoadCodeSnippet(const char* filename);
	
#if defined (USE_ProgrammablePipeLine)
	//default shaders
	gxGenericShader m_cOnlyDiffuse;
	gxGenericShader m_cDiffuseUnlit;
	gxGenericShader m_cOnlyDiffuseWithColor;
	gxGenericShader m_cGenericShader;
	gxGenericShader m_cLightingOnlyGenericShader;
	gxGenericShader m_cLightingOnlyFirstPassGenericShader;
	gxGenericShader m_cSpriteGenericShader;
	gxGenericShader m_cBlurGenericShader;
	std::vector<gxHWShader*> m_cvHWShaderLst;

	std::vector<stHWShaderSnippet*> m_cvHWShaderSnippets;
#endif
};

#endif
