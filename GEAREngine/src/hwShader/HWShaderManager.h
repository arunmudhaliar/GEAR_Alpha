#ifndef HWSHADERMANAGER_H
#define HWSHADERMANAGER_H

#include "../renderer/gxRenderer.h"
#include <vector>
#include "gxHWShader.h"

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
	gxHWShader m_cOnlyDiffuse;
	gxHWShader m_cDiffuseUnlit;
	gxHWShader m_cOnlyDiffuseWithColor;
	gxHWShader m_cGenericShader;
	gxHWShader m_cLightingOnlyGenericShader;
	gxHWShader m_cLightingOnlyFirstPassGenericShader;
	gxHWShader m_cSpriteGenericShader;
	gxHWShader m_cBlurGenericShader;
	std::vector<gxHWShader*> m_cvHWShaderLst;

	std::vector<stHWShaderSnippet*> m_cvHWShaderSnippets;
#endif
};

#endif
