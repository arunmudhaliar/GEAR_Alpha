#ifndef BLURFILTER_H
#define BLURFILTER_H

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
	gxHWShader* m_pBlurGLSLShaderPtr;
	FBO* m_pFBOInput;
};
#endif