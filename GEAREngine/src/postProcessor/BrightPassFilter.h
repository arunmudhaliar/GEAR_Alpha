#pragma once

#include "PostProcessor.h"
#include "../hwShader/HWShaderManager.h"

class DECLSPEC BrightPassFilter : public PostProcessor
{
public:
	BrightPassFilter();
	virtual ~BrightPassFilter();

	virtual void beginBlit();
	virtual void blit(gxRenderer* renderer);
	virtual void endBlit();
	virtual void resize(float cx, float cy);

	void init(FBO* input);
protected:
	gxHWShader* brightPassGLSLShader;
	FBO* inputFbo;
};