#ifndef TONEMAPPINGFILTER_H
#define TONEMAPPINGFILTER_H

#include "PostProcessor.h"
#include "../hwShader/HWShaderManager.h"

class DECLSPEC ToneMappingFilter : public PostProcessor
{
public:
	ToneMappingFilter();
	virtual ~ToneMappingFilter();

	virtual void beginBlit();
	virtual void blit(gxRenderer* renderer);
	virtual void endBlit();

	void init(FBO* sceneInput, FBO* blurSceneInput);
protected:
	gxHWShader* m_pToneMappingGLSLShaderPtr;
	FBO* m_pSceneFBOInput;
	FBO* m_pBlurSceneFBOInput;
};
#endif