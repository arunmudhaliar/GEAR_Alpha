#include "ToneMappingFilter.h"
#include "../GEAREngine.h"

ToneMappingFilter::ToneMappingFilter() :
PostProcessor("ToneMappingFilter", 512, 512)
{
	tonemappingGLSLShader = NULL;
	inputFbo = NULL;
	inputBlutSceneFbo = NULL;
}

ToneMappingFilter::~ToneMappingFilter()
{
}

void ToneMappingFilter::init(FBO* sceneInput, FBO* blurSceneInput)
{
	inputFbo = sceneInput;
	inputBlutSceneFbo = blurSceneInput;
	tonemappingGLSLShader = engine_getHWShaderManager()->LoadShaderFromFile("hwshader/tonemappingshader.glsl");

	fbo.BindFBO();
    fbo.CreateTextureBuffer();
    fbo.AttachTextureBuffer(0);
	fbo.CreateDepthBuffer();
	fbo.AttachDepthBuffer();
	fbo.UnBindFBO();

}

void ToneMappingFilter::beginBlit()
{
	PostProcessor::beginBlit();
	tonemappingGLSLShader->enableProgram();
	CHECK_GL_ERROR(glEnable(GL_DEPTH_TEST));
	CHECK_GL_ERROR(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void ToneMappingFilter::endBlit()
{
	tonemappingGLSLShader->disableProgram();
	PostProcessor::endBlit();
}

void ToneMappingFilter::resize(float cx, float cy)
{
	PostProcessor::resize(cx, cy);
    fbo.CreateTextureBuffer();
    fbo.AttachTextureBuffer(0);
	fbo.CreateDepthBuffer();
	fbo.AttachDepthBuffer();
}

void ToneMappingFilter::blit(gxRenderer* renderer)
{
	float cx = fbo.getFBOWidth();
	float cy = fbo.getFBOHeight();

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

	CHECK_GL_ERROR(glVertexAttribPointer(tonemappingGLSLShader->getAttribLoc("a_vertex_coord_v4"), 2, GL_FLOAT, GL_FALSE, 0, cszTileBuffer));
	CHECK_GL_ERROR(glEnableVertexAttribArray(tonemappingGLSLShader->getAttribLoc("a_vertex_coord_v4")));

	////////
	CHECK_GL_ERROR(glActiveTexture(GL_TEXTURE0));
	CHECK_GL_ERROR(glVertexAttribPointer(tonemappingGLSLShader->getAttribLoc("a_uv_coord0_v2"), 2, GL_FLOAT, GL_FALSE, 0, cszTileTexBuffer));
	CHECK_GL_ERROR(glEnableVertexAttribArray(tonemappingGLSLShader->getAttribLoc("a_uv_coord0_v2")));
	//arun:texissue glEnable(GL_TEXTURE_2D);
	CHECK_GL_ERROR(glBindTexture(GL_TEXTURE_2D, inputFbo->getFBOTextureBuffer(0)));
	tonemappingGLSLShader->sendUniform1i("u_diffuse_texture", 0);

	CHECK_GL_ERROR(glActiveTexture(GL_TEXTURE0+1));
	CHECK_GL_ERROR(glBindTexture(GL_TEXTURE_2D, inputBlutSceneFbo->getFBOTextureBuffer(0)));
	tonemappingGLSLShader->sendUniform1i("u_bloomblur_texture", 1);

	tonemappingGLSLShader->sendUniform1f("u_exposure", 0.9f);

	matrix4x4f offset;
	float x, y;
	x = y = 0.0f;
	offset.setPosition(-(-x + cx)*0.5f, -(-y + cy /*+ getTopMarginOffsetHeight()*/)*0.5f, -1.0f);
	const float* u_mvp_m4x4 = (*renderer->getOrthoProjectionMatrix() * offset).getMatrix();
	tonemappingGLSLShader->sendUniformTMfv("u_mvp_m4x4", u_mvp_m4x4, false, 4);

	CHECK_GL_ERROR(glDrawArrays(GL_TRIANGLE_FAN, 0, 4));

	//Disable all texture ops
	CHECK_GL_ERROR(glActiveTexture(GL_TEXTURE0));
	CHECK_GL_ERROR(glDisableVertexAttribArray(tonemappingGLSLShader->getAttribLoc("a_uv_coord0_v2")));
	//arun:texissue glDisable(GL_TEXTURE_2D);
	CHECK_GL_ERROR(glBindTexture(GL_TEXTURE_2D, 0));

	CHECK_GL_ERROR(glActiveTexture(GL_TEXTURE0+1));
	CHECK_GL_ERROR(glBindTexture(GL_TEXTURE_2D, 0));

	CHECK_GL_ERROR(glDisableVertexAttribArray(tonemappingGLSLShader->getAttribLoc("a_vertex_coord_v4")));
	CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, 0));
}