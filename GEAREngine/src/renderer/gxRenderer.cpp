#include "gxRenderer.h"

gxRenderer::gxRenderer()
{
	m_pProjectionMatrixPtr=NULL;
	m_pViewMatrixPtr=NULL;
	m_pViewProjectionMatrixPtr=NULL;
	m_pGEARTexture1x1Ptr=NULL;
	m_cViewPortRect.set(0.0f, 0.0f, 1.0f, 1.0f);
	m_nTrisRendered=0;
	m_nDrawCalls=0;
	setRenderPassType(RENDER_NORMAL);
}

gxRenderer::~gxRenderer()
{
}

void gxRenderer::setViewPort(float x, float y, float cx, float cy)
{
	if(cx<=1.0f)
		cx=1.0f;
	if(cy<=1.0f)
		cy=1.0f;

	gxRectf viewportRect(x, y, cx, cy);
	vector2f centerAlignedPos(viewportRect.m_pos-viewportRect.m_size*0.5f);
	m_cOrthogonalProjectionMatrix.setOrtho(centerAlignedPos.x, centerAlignedPos.x+viewportRect.m_size.x, centerAlignedPos.y+viewportRect.m_size.y, centerAlignedPos.y, -100.0f, 1000.0f);
	m_cViewPortRect.set(x, y, cx, cy);
	CHECK_GL_ERROR(glViewport((int)x, (int)y, (int)cx, (int)cy));
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