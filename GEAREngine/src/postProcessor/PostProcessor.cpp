#include "PostProcessor.h"

PostProcessor::PostProcessor(std::string name, int fbo_cx, int fbo_cy) :
m_cName(name)
{
	m_cFBO.ReInitFBO(fbo_cx, fbo_cy);
	m_cFBO.UnBindFBO();
}

PostProcessor::~PostProcessor()
{

}

void PostProcessor::beginBlit()
{
	CHECK_GL_ERROR(glPushAttrib(GL_VIEWPORT_BIT));
	m_cFBO.BindFBO();
	CHECK_GL_ERROR(glViewport(0, 0, m_cFBO.getFBOWidth(), m_cFBO.getFBOHeight()));
}

void PostProcessor::endBlit()
{
	m_cFBO.UnBindFBO();
	CHECK_GL_ERROR(glPopAttrib());
}

void PostProcessor::resize(float cx, float cy)
{
	m_cFBO.ReInitFBO(cx, cy);
}