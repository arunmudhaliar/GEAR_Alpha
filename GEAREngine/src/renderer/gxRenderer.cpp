#include "gxRenderer.h"

gxRenderer::gxRenderer()
{
	m_pProjectionMatrixPtr=NULL;
	m_pViewMatrixPtr=NULL;
	m_pViewProjectionMatrixPtr=NULL;
	m_cViewPortRect.set(0.0f, 0.0f, 1.0f, 1.0f);
}

gxRenderer::~gxRenderer()
{
}

void gxRenderer::setViewPort(float x, float y, float cx, float cy)
{
    m_cOrthogonalProjectionMatrix.setOrtho(0.0f, cx, cy, 0.0f, 0.0f, 10.0f);
	m_cViewPortRect.set(x, y, cx, cy);
	glViewport((int)x, (int)y, (int)cx, (int)cy);
}

void gxRenderer::setProjectionMatrixToGL(matrix4x4f* matrix)
{
	m_pProjectionMatrixPtr=matrix;
#if defined (USE_ProgrammablePipeLine)
#else
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(matrix->getMatrix());
    glMatrixMode(GL_MODELVIEW);
#endif
}

void gxRenderer::setViewMatrixToGL(matrix4x4f* matrix)
{
	m_pViewMatrixPtr=matrix;
#if defined (USE_ProgrammablePipeLine)
#else
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(matrix->getMatrix());
#endif
}

void gxRenderer::setViewProjectionMatrix(matrix4x4f* matrix)
{
	m_pViewProjectionMatrixPtr=matrix;
}