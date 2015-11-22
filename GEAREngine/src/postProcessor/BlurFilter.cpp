#include "BlurFilter.h"
#include "../GEAREngine.h"

BlurFilter::BlurFilter() :
PostProcessor("BlurFilter", 512, 512)
{
	m_pBlurGLSLShaderPtr = NULL;
	m_pFBOInput = NULL;
}

BlurFilter::~BlurFilter()
{
}

void BlurFilter::init(FBO* input)
{
	m_pFBOInput = input;
	m_pBlurGLSLShaderPtr = engine_getHWShaderManager()->LoadShaderFromFile("hwshader/blurshader.glsl");

	m_cFBO.BindFBO();
	m_cFBO.CreateDepthBuffer();
	m_cFBO.AttachDepthBuffer();
	m_cFBO.CreateTextureBuffer();
	m_cFBO.AttachTextureBuffer(0);
	m_cFBO.UnBindFBO();

}

void BlurFilter::beginBlit()
{
	PostProcessor::beginBlit();
	m_pBlurGLSLShaderPtr->enableProgram();
	CHECK_GL_ERROR(glEnable(GL_DEPTH_TEST));
	CHECK_GL_ERROR(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void BlurFilter::endBlit()
{
	m_pBlurGLSLShaderPtr->disableProgram();
	PostProcessor::endBlit();
}

void BlurFilter::blit(gxRenderer* renderer)
{
	float cx = m_cFBO.getFBOWidth();
	float cy = m_cFBO.getFBOHeight();

	float cszTileBuffer[] = {
		0, 0,
		0, 0 + cy,
		0 + cx, 0 + cy,
		0 + cx, 0
	};

	float cszTileTexBuffer[] = {
		0, 1,
		0, 0,
		1, 0,
		1, 1,
	};

	CHECK_GL_ERROR(glEnable(GL_CULL_FACE));

	CHECK_GL_ERROR(glVertexAttribPointer(m_pBlurGLSLShaderPtr->getAttribLoc("a_vertex_coord_v4"), 2, GL_FLOAT, GL_FALSE, 0, cszTileBuffer));
	CHECK_GL_ERROR(glEnableVertexAttribArray(m_pBlurGLSLShaderPtr->getAttribLoc("a_vertex_coord_v4")));

	////////
	CHECK_GL_ERROR(glActiveTexture(GL_TEXTURE0));
	CHECK_GL_ERROR(glVertexAttribPointer(m_pBlurGLSLShaderPtr->getAttribLoc("a_uv_coord0_v2"), 2, GL_FLOAT, GL_FALSE, 0, cszTileTexBuffer));
	CHECK_GL_ERROR(glEnableVertexAttribArray(m_pBlurGLSLShaderPtr->getAttribLoc("a_uv_coord0_v2")));
	//arun:texissue glEnable(GL_TEXTURE_2D);
	CHECK_GL_ERROR(glBindTexture(GL_TEXTURE_2D, m_pFBOInput->getFBOTextureBuffer(0)));

	m_pBlurGLSLShaderPtr->sendUniform1i("u_diffuse_texture", 0);

	matrix4x4f offset;
	float x, y;
	x = y = 0.0f;
	offset.setPosition(-(-x + cx)*0.5f, -(-y + cy /*+ getTopMarginOffsetHeight()*/)*0.5f, -1.0f);
	const float* u_mvp_m4x4 = (*renderer->getOrthoProjectionMatrix() * offset).getMatrix();
	m_pBlurGLSLShaderPtr->sendUniformTMfv("u_mvp_m4x4", u_mvp_m4x4, false, 4);

	CHECK_GL_ERROR(glDrawArrays(GL_TRIANGLE_FAN, 0, 4));

	//Disable all texture ops
	CHECK_GL_ERROR(glDisableVertexAttribArray(m_pBlurGLSLShaderPtr->getAttribLoc("a_uv_coord0_v2")));
	//arun:texissue glDisable(GL_TEXTURE_2D);
	CHECK_GL_ERROR(glBindTexture(GL_TEXTURE_2D, 0));

	CHECK_GL_ERROR(glDisableVertexAttribArray(m_pBlurGLSLShaderPtr->getAttribLoc("a_vertex_coord_v4")));
	CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, 0));
}