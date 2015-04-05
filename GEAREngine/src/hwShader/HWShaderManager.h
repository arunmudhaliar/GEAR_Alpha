#ifndef HWSHADERMANAGER_H
#define HWSHADERMANAGER_H

#include "../renderer/gxRenderer.h"
#include <vector>
#include "gxHWShader.h"
#include "../core/gxSurfaceShader.h"

#define HW_BUILTIN_DEFAULT_SHADER_ONLY_DIFFUSE					0
#define HW_BUILTIN_DEFAULT_SHADER_ONLY_DIFFUSE_WITH_COLOR_PTR	1
#define HW_BUILTIN_DEFAULT_SHADER_ONLY_GUI_SHADER				2
#define HW_BUILTIN_DEFAULT_SHADER_ONLY_BLUR_SHADER				3
#define HW_BUILTIN_DEFAULT_SHADER_ONLY_SHADOWMAP_SHADER			4
#define HW_BUILTIN_DEFAULT_SHADER_ONLY_FOG_LINEAR_SHADER		5
#define HW_BUILTIN_DEFAULT_SHADER_ONLY_FOG_EXP_SHADER			6
#define HW_BUILTIN_DEFAULT_SHADER_ONLY_FOG_EXP2_SHADER			7

class DECLSPEC HWShaderManager
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
		long size;
	};

	HWShaderManager();
	~HWShaderManager();
	
	void Init();
	void Reset();
    void update(float dt);
	
#if defined (USE_ProgrammablePipeLine)
	gxHWShader* GetHWShader(int index);
#endif
    
	stHWShaderSnippet* getShaderSnippet(int index)	{	return m_cvHWShaderSnippets[index];	}
	gxHWShader* LoadShaderFromBuffer(const char* name, const char* buffer, int size);

	gxSurfaceShader* LoadSurfaceShader(const char* filename);
	std::vector<gxSurfaceShader*>* getSurfaceShaderList()	{	return &m_cvHWSurfaceShader;	}

private:
	void LoadDefaultShaders();
	stHWShaderSnippet* LoadCodeSnippet(const char* filename);
	
#if defined (USE_ProgrammablePipeLine)
	std::vector<gxSurfaceShader*> m_cvHWSurfaceShader;
	std::vector<gxHWShader*> m_cvHWShaderLst;
	std::vector<stHWShaderSnippet*> m_cvHWShaderSnippets;
#endif
};

#endif
