#ifndef POSTPROCESSOR_H
#define POSTPROCESSOR_H

#include "../core/basicIncludes.h"
#include "../renderer/gxRenderer.h"
#include "../core/fbo.h"
#include <string>

class DECLSPEC PostProcessor
{
public:
	PostProcessor(std::string name, int fbo_cx, int fbo_cy);
	virtual ~PostProcessor();

	virtual void beginBlit();
	virtual void blit(gxRenderer* renderer)=0;
	virtual void endBlit();

	virtual void resize(float cx, float cy);

	std::string getName()	{ return m_cName;	 }
	FBO& getOutPutFBO()		{ return m_cFBO;	 }

protected:
	std::string m_cName;
	FBO m_cFBO;

};
#endif