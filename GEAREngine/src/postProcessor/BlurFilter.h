#pragma once

#include "PostProcessor.h"
#include "../hwShader/HWShaderManager.h"

class DECLSPEC BlurFilter : public PostProcessor
{
public:
	BlurFilter();
	virtual ~BlurFilter();

	virtual void beginBlit();
	virtual void blit(gxRenderer* renderer);
	virtual void endBlit();
	virtual void resize(float cx, float cy);

	void init(FBO* input);
protected:
	gxHWShader* blurGLSLShader;
	FBO* inputFbo;
};