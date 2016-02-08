#pragma once

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
	virtual void resize(float cx, float cy);

	void init(FBO* sceneInput, FBO* blurSceneInput);
protected:
	gxHWShader* tonemappingGLSLShader;
	FBO* inputFbo;
	FBO* inputBlutSceneFbo;
};