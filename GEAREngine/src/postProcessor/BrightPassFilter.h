#ifndef BRIGHTPASSFILTER_H
#define BRIGHTPASSFILTER_H

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

	void init(FBO* input);
protected:
	gxHWShader* m_pBrightPassGLSLShaderPtr;
	FBO* m_pFBOInput;
};
#endif