#include "PostProcessor.h"

PostProcessor::PostProcessor(std::string name, int fbo_cx, int fbo_cy) :
m_cName(name)
{
	fbo.ReInitFBO(fbo_cx, fbo_cy);
	fbo.UnBindFBO();
}

PostProcessor::~PostProcessor()
{

}

void PostProcessor::beginBlit()
{
	CHECK_GL_ERROR(glPushAttrib(GL_VIEWPORT_BIT));
	fbo.BindFBO();
	CHECK_GL_ERROR(glViewport(0, 0, fbo.getFBOWidth(), fbo.getFBOHeight()));
}

void PostProcessor::endBlit()
{
	fbo.UnBindFBO();
	CHECK_GL_ERROR(glPopAttrib());
}

void PostProcessor::resize(float cx, float cy)
{
	fbo.ReInitFBO(cx, cy);
}