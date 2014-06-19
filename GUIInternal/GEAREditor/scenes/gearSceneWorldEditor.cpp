#include "gearSceneWorldEditor.h"
#include "../EditorApp.h"
//#include "../../../GEAREngine/src/core/Timer.h"
#include "../gui/geToolBarSeperator.h"
#include "../../../GEAREngine/src/hwShader/gxHWShader.h"
#include "../../resource.h"

#include "../../../GEAREngine/src/util/nvProfiler.h"
#include "../../../GEAREngine/src/util/nvProfiler.cpp"

//void drawRoundedRectangle(float x, float y, float cx, float cy, float deltaHeight);

gearSceneWorldEditor::gearSceneWorldEditor():
geWindow("World Editor")
{
	m_pSelectedObj=NULL;
	m_pMainWorldPtr=NULL;

	m_pHorizontalSlider_LightAmbient=NULL;
	m_pHorizontalSlider_TimeScale=NULL;
	m_pLocalOrGlobalAxis=NULL;
	m_pTBGridView=NULL;

	m_pPlayButton = NULL;
	m_pPauseButton = NULL;
	m_pTBOnlyLightPass = NULL;
	m_pTBShowOOBB = NULL;
	m_pTBShowOctree = NULL;
	m_bMonoGameInitialized=false;

	m_pTranslateGizmo = NULL;
	m_pRotateGizmo = NULL;
	m_pScaleGizmo = NULL;
	m_bEnablePostProcessorBlur=false;
	m_iLastGLError=0;
	m_bStopFollowCam=true;
}

gearSceneWorldEditor::~gearSceneWorldEditor()
{
#if USE_NVPROFILER
	GetNvPmApi()->Shutdown();
#endif
	//GE_DELETE(m_pHorizontalSlider_LightAmbient);
}

void gearSceneWorldEditor::onCreate()
{
#if USE_NVPROFILER
	nvProfiler::InitNVPM(EditorApp::getMainRenderer()->getRenderingContext());
#endif

	monoWrapper::mono_engine_init(2);
	m_pMainWorldPtr = monoWrapper::mono_engine_getWorld(0);

	m_pMainWorldPtr->setWorldObserver(this);

	char metaDataFolder[512];
	memset(metaDataFolder, 0, sizeof(metaDataFolder));
	sprintf(metaDataFolder, "%s/%s", EditorApp::getProjectHomeDirectory(), "MetaData");
	monoWrapper::mono_engine_setMetaFolder(monoWrapper::mono_engine_getWorld(0), metaDataFolder);
	monoWrapper::mono_engine_setMetaFolder(monoWrapper::mono_engine_getWorld(1), metaDataFolder);

	m_pLocalOrGlobalAxis=new geToolBarButton(m_pRenderer, "Local", getToolBar());
	m_pLocalOrGlobalAxis->setGUIObserver(this);
	getToolBar()->appendToolBarControl(m_pLocalOrGlobalAxis);
	m_bTransformThroughLocalAxis=true;

	m_pTranslateGizmo=new geToolBarButton(m_pRenderer, "t", getToolBar());
	m_pTranslateGizmo->loadImage("res//icons16x16.png", 7, 153);
	m_pTranslateGizmo->setGUIObserver(this);
	getToolBar()->appendToolBarControl(m_pTranslateGizmo);
	m_pRotateGizmo=new geToolBarButton(m_pRenderer, "r", getToolBar());
	m_pRotateGizmo->loadImage("res//icons16x16.png", 27, 153);
	m_pRotateGizmo->setGUIObserver(this);
	getToolBar()->appendToolBarControl(m_pRotateGizmo);
	m_pScaleGizmo=new geToolBarButton(m_pRenderer, "s", getToolBar());
	m_pScaleGizmo->loadImage("res//icons16x16.png", 49, 153);
	m_pScaleGizmo->setGUIObserver(this);
	getToolBar()->appendToolBarControl(m_pScaleGizmo);

	//enable translate gizmo
	m_pTranslateGizmo->buttonPressed(false);

	m_pHorizontalSlider_LightAmbient = new geHorizontalSlider();
	m_pHorizontalSlider_LightAmbient->create(m_pRenderer, getToolBar(), "slider", 0, GE_TOOLBAR_HEIGHT*0.35f, 70);
	m_pHorizontalSlider_LightAmbient->setGUIObserver(this);
	m_pHorizontalSlider_LightAmbient->setSliderValue(0.4f);
	getToolBar()->appendToolBarControl(m_pHorizontalSlider_LightAmbient);

	geToolBarSeperator* seperator = new geToolBarSeperator(m_pRenderer, getToolBar(), 20);
	getToolBar()->appendToolBarControl(seperator);
	geToolBarSeperator* seperator2 = new geToolBarSeperator(m_pRenderer, getToolBar(), 40);
	getToolBar()->appendToolBarControl(seperator2);

	m_pTBGridView=new geToolBarButton(m_pRenderer, "grid", getToolBar());
	m_pTBGridView->loadImage("res//icons16x16.png", 112, 384);
	//m_pTBGridView->setClientAreaSecondryActiveForeColor(0.5f, 0.3f, 0.2f, 1.0f);
	//m_pTBGridView->applyPrimaryColorToVBClientArea(EGRADIENT_VERTICAL_UP, 0.3f);
	m_pTBGridView->buttonPressed(false);
	getToolBar()->appendToolBarControl(m_pTBGridView);

	geToolBarSeperator* seperator3 = new geToolBarSeperator(m_pRenderer, getToolBar(), 40);
	getToolBar()->appendToolBarControl(seperator3);

	//play-pause-stop
	m_pPlayButton=new geToolBarButton(m_pRenderer, "play", getToolBar());
	m_pPlayButton->loadImage("res//icons16x16.png", 26, 216);
	m_pPlayButton->setGUIObserver(this);
	getToolBar()->appendToolBarControl(m_pPlayButton);
	m_pPauseButton=new geToolBarButton(m_pRenderer, "pause", getToolBar());
	m_pPauseButton->loadImage("res//icons16x16.png", 90, 216);
	m_pPauseButton->setGUIObserver(this);
	getToolBar()->appendToolBarControl(m_pPauseButton);
	//

	geToolBarSeperator* seperator4 = new geToolBarSeperator(m_pRenderer, getToolBar(), 40);
	getToolBar()->appendToolBarControl(seperator4);

	m_pHorizontalSlider_TimeScale = new geHorizontalSlider();
	m_pHorizontalSlider_TimeScale->create(m_pRenderer, getToolBar(), "slider", 0, GE_TOOLBAR_HEIGHT*0.35f, 130);
	m_pHorizontalSlider_TimeScale->setSliderValue(1.0f);
	getToolBar()->appendToolBarControl(m_pHorizontalSlider_TimeScale);

	geToolBarSeperator* seperator5 = new geToolBarSeperator(m_pRenderer, getToolBar(), 40);
	getToolBar()->appendToolBarControl(seperator5);

	m_pTBOnlyLightPass=new geToolBarButton(m_pRenderer, "onlylightpass", getToolBar());
	m_pTBOnlyLightPass->loadImage("res//icons16x16.png", 153, 342);
	m_pTBOnlyLightPass->setGUIObserver(this);
	getToolBar()->appendToolBarControl(m_pTBOnlyLightPass);

	m_pTBShowOOBB=new geToolBarButton(m_pRenderer, "showoobb", getToolBar());
	m_pTBShowOOBB->loadImage("res//icons16x16.png", 133, 258);
	m_pTBShowOOBB->setGUIObserver(this);
	getToolBar()->appendToolBarControl(m_pTBShowOOBB);

	m_pTBShowOctree=new geToolBarButton(m_pRenderer, "showoctree", getToolBar());
	m_pTBShowOctree->loadImage("res//icons16x16.png", 70, 69);
	m_pTBShowOctree->setGUIObserver(this);
	getToolBar()->appendToolBarControl(m_pTBShowOctree);

	//create grid
	float startX=-500.0f;
	float startY=-500.0f;

	float oneGridSz=abs(2*startX)/10.0f;
	float oneSubGridSz=oneGridSz/10.0f;
	float halfoneSubGridSz=0;//oneSubGridSz*0.5f;

	m_cGridOuterBox[0].set(startX, startY);
	m_cGridOuterBox[1].set(-startX, startY);
	m_cGridOuterBox[2].set(-startX, -startY);
	m_cGridOuterBox[3].set(startX, -startY);

	for(int x=0,cntrY=0, cntrX=0;x<10;x++)
	{
		if(x<9)
		{
			m_cThickGridOnYAxis[x*2+0].x=startX+halfoneSubGridSz+(x+1)*oneGridSz;
			m_cThickGridOnYAxis[x*2+0].y=startY;

			m_cThickGridOnYAxis[x*2+1].x=startX+halfoneSubGridSz+(x+1)*oneGridSz;
			m_cThickGridOnYAxis[x*2+1].y=-startY;

			m_cThickGridOnXAxis[x*2+0].x=startX;
			m_cThickGridOnXAxis[x*2+0].y=startY+halfoneSubGridSz+(x+1)*oneGridSz;

			m_cThickGridOnXAxis[x*2+1].x=-startX;
			m_cThickGridOnXAxis[x*2+1].y=startY+halfoneSubGridSz+(x+1)*oneGridSz;
		}

		for(int y=0;y<9;y++)
		{
			m_cGridOnYAxis[cntrY].x=startX+oneSubGridSz+x*oneGridSz+y*oneSubGridSz;
			m_cGridOnYAxis[cntrY++].y=startY;

			m_cGridOnYAxis[cntrY].x=startX+oneSubGridSz+x*oneGridSz+y*oneSubGridSz;
			m_cGridOnYAxis[cntrY++].y=-startY;

			m_cGridOnXAxis[cntrX].x=startX;
			m_cGridOnXAxis[cntrX++].y=startY+oneSubGridSz+x*oneGridSz+y*oneSubGridSz;

			m_cGridOnXAxis[cntrX].x=-startX;
			m_cGridOnXAxis[cntrX++].y=startY+oneSubGridSz+x*oneGridSz+y*oneSubGridSz;
		}
	}
	//

	//hwshader manager
	m_pHWShaderManager = engine_getHWShaderManager();
	m_pHWShaderManager->Init();

#if defined USE_FBO
	//fbo
	m_cMultiPassFBO.ReInitFBO(512, 512);
    m_cMultiPassFBO.CreateDepthBuffer();
    m_cMultiPassFBO.AttachDepthBuffer();
    m_cMultiPassFBO.CreateTextureBuffer();
    m_cMultiPassFBO.AttachTextureBuffer(0);
    m_cMultiPassFBO.UnBindFBO();

	m_cShadowMapFBO.ReInitFBO(512, 512);
    m_cShadowMapFBO.CreateDepthBuffer();
    m_cShadowMapFBO.AttachDepthBuffer();
	m_cShadowMapFBO.CreateDepthShadowTextureBuffer();
    m_cShadowMapFBO.AttachShadowTextureBuffer();
    m_cShadowMapFBO.UnBindFBO();
#endif

	m_cLightBillBoardSprite.setOffset(0, 0);
	m_cLightBillBoardSprite.loadTexture(&geGUIManager::g_cTextureManager, "res//icons16x16.png");

	//m_cSoundEngine.init();
	//m_cSoundEngine.createListener();
	//SoundSource* snd=m_cSoundEngine.load("res//sounds//ingame_level5_44_16_s.sx");
	//snd->play();
}


vector3f minV;
vector3f maxV;
vector3f i1, i2, testSpehere;

void gearSceneWorldEditor::draw()
{
#ifdef LOG_GLERROR
	int err=glGetError();
	if(err!=GL_NO_ERROR)
	{
		printf("glGetError 0x%x\n", err);
	}
#endif

	//CHECK_GL_ERROR(drawShadowMapPass());
	CHECK_GL_ERROR(drawLightsOnMultiPass());
	CHECK_GL_ERROR(onDraw());

#if defined USE_FBO
	//if(bMultiPass)
		//m_cMultiPassFBO.UnBindFBO();
	//m_cFBO.UnBindFBO();
#endif

	CHECK_GL_ERROR(drawStats());

#if USE_BULLET
	glViewport(m_cPos.x+getIamOnLayout()->getPos().x, (m_pRenderer->getViewPortSz().y)-(m_cPos.y+getIamOnLayout()->getPos().y+m_cSize.y), m_cSize.x, m_cSize.y-getTopMarginOffsetHeight());	
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluPerspective(45, 1, 1, 1000);
	glLoadMatrixf(monoWrapper::mono_engine_getWorld(0)->getRenderer()->getProjectionMatrix()->getMatrix());
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadMatrixf(monoWrapper::mono_engine_getWorld(0)->getRenderer()->getViewMatrix()->getMatrix());

	CHECK_GL_ERROR(monoWrapper::mono_engine_getWorld(0)->getPhysicsEngine()->render());
	glPopMatrix();
#endif
}

void gearSceneWorldEditor::drawFBO(GLuint t, float x, float y, float cx, float cy)
{
    float cszTileBuffer[]={
        x,      y,
        x,      y+cy,
        x+cx,   y+cy,
        x+cx,   y
    };
    
	float cszTileTexBuffer[]={
        0,1,
		0,0,
		1,0,
		1,1,
    };
  
	if(m_bEnablePostProcessorBlur)
	{
		gxHWShader* shader=engine_getHWShaderManager()->GetHWShader(HW_BUILTIN_DEFAULT_SHADER_ONLY_BLUR_SHADER);
    
		shader->enableProgram();
   
		glVertexAttribPointer(shader->getAttribLoc("a_vertex_coord_v4"), 2, GL_FLOAT, GL_FALSE, 0, cszTileBuffer);
		glEnableVertexAttribArray(shader->getAttribLoc("a_vertex_coord_v4"));
    
    
		////////

		glActiveTexture(GL_TEXTURE0);
		glVertexAttribPointer(shader->getAttribLoc("a_uv_coord0_v2"), 2, GL_FLOAT, GL_FALSE, 0, cszTileTexBuffer);
		glEnableVertexAttribArray(shader->getAttribLoc("a_uv_coord0_v2"));
		//arun:texissue glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, t);	

		shader->sendUniform1i("u_diffuse_texture", 0);

		matrix4x4f cRenderMatrix;
		const float* u_mvp_m4x4= m_pMainWorldPtr->getRenderer()->getOrthoProjectionMatrix()->getMatrix();
		shader->sendUniformTMfv("u_mvp_m4x4", u_mvp_m4x4, false, 4);
    
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    
		//Disable all texture ops
		glDisableVertexAttribArray(shader->getAttribLoc("a_uv_coord0_v2"));
		//arun:texissue glDisable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
    
		glDisableVertexAttribArray(shader->getAttribLoc("a_vertex_coord_v4"));
		glBindBuffer(GL_ARRAY_BUFFER,0);
    
		shader->disableProgram();    
	}
	else
	{
		glColor3f(1.0f, 1.0f, 1.0f);
 		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_FLOAT, 0, cszTileBuffer);
    
		glActiveTexture(GL_TEXTURE0);
		glClientActiveTexture(GL_TEXTURE0);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, 0, cszTileTexBuffer);
    
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, t);
		//glTranslatef(0, 0, 0);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
	}
}

void gearSceneWorldEditor::onDraw()
{
	followObject(Timer::getDtinSec(), m_pSelectedObj);

	monoWrapper::mono_engine_update(m_pMainWorldPtr, Timer::getDtinSec()*m_pHorizontalSlider_TimeScale->getSliderValue());
	if(m_pPlayButton->isButtonPressed() && !m_pPauseButton->isButtonPressed())
	{
		if(m_bMonoGameInitialized)
			monoWrapper::mono_game_run(Timer::getDtinSec()*m_pHorizontalSlider_TimeScale->getSliderValue());
	}
}

void gearSceneWorldEditor::drawShadowMapPass()
{
	std::vector<gxLight*>* lightList = m_pMainWorldPtr->getLightList();
	if(lightList->size()==0)
		return;

	gxLight* light=lightList->at(0);

	m_cShadowMapFBO.BindFBO();
	//projection
	//glm::vec3 lightInvDir = glm::vec3(0.5f,2,2);
	//// Compute the MVP matrix from the light's point of view
	//glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10,10,-10,10,-10,20);
	//glm::mat4 depthViewMatrix = glm::lookAt(lightInvDir, glm::vec3(0,0,0), glm::vec3(0,1,0));
	//glm::mat4 depthModelMatrix = glm::mat4(1.0);
	//glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;
	//
	matrix4x4f depthProjectionMatrix;//=m_pMainWorldPtr->getRenderer()->getOrthoProjectionMatrix();
	depthProjectionMatrix.setOrtho(-512, 512, -512, 512, 0, 10000);
	matrix4x4f depthViewMatrix;
	//vector3f up(1, 0, 0);
	vector3f forward(-light->getPosition());
    forward.normalize();
    vector3f up(0, 0, 1);
    vector3f left(up.cross(forward));
    left.normalize();
    up=forward.cross(left);
    up.normalize();
	depthViewMatrix.setXAxis(left);
	depthViewMatrix.setYAxis(up);
	depthViewMatrix.setZAxis(forward);
	//depthViewMatrix.setPosition(light->getPosition());

	matrix4x4f depthModelMatrix;
	depthModelMatrix.setPosition(light->getPosition());

	//depthViewMatrix.setDirection(&-light->getPosition(), &light->getYAxis());
	matrix4x4f depthMVP(depthProjectionMatrix * depthViewMatrix.getInverse() * depthModelMatrix);

	//const float* u_mvp_m4x4= (*m_pMainWorldPtr->getRenderer()->getViewProjectionMatrix() * (*light->getWorldMatrix() * m_cLightBillBoardSprite)).getMatrix();

	CHECK_GL_ERROR(glEnable(GL_DEPTH_TEST));
	CHECK_GL_ERROR(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	m_pMainWorldPtr->getRenderer()->setRenderPassType(gxRenderer::RENDER_SHADOWMAP);

	gxHWShader* shader=engine_getHWShaderManager()->GetHWShader(HW_BUILTIN_DEFAULT_SHADER_ONLY_SHADOWMAP_SHADER);
	shader->enableProgram();
	shader->sendUniformTMfv("u_depth_mvp_m4x4", depthMVP.getOGLMatrix(), false, 4);
	m_pMainWorldPtr->renderShadow(m_pMainWorldPtr->getRenderer());
	m_pMainWorldPtr->getRenderer()->setRenderPassType(gxRenderer::RENDER_NORMAL);

	shader->disableProgram();
	m_cShadowMapFBO.UnBindFBO();
}

void gearSceneWorldEditor::drawLightsOnMultiPass()
{
	m_cMultiPassFBO.BindFBO();
	//gxCamera* active_cam=m_pMainWorldPtr->getActiveCamera()->getCameraStructure();
	monoWrapper::mono_engine_resize(m_pMainWorldPtr, m_cPos.x+getIamOnLayout()->getPos().x, (m_pRenderer->getViewPortSz().y)-(m_cPos.y+getIamOnLayout()->getPos().y+m_cSize.y), m_cSize.x/*+2.0f*/, m_cSize.y-getTopMarginOffsetHeight()/**//*+2.0f*/, 10.0f, 100000.0f);
    CHECK_GL_ERROR(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

#if USE_NVPROFILER
	if(nvProfiler::g_bPerformanceAnalyze)
	{
		NVPMUINT unNumPasses = 1;
		// Get the number of passes we'll need for all experiments
		GetNvPmApi()->BeginExperiment(nvProfiler::g_hNVPMContext, &unNumPasses);
		
		for(int npass=0;npass<unNumPasses; npass++)
		{
			GetNvPmApi()->BeginPass(nvProfiler::g_hNVPMContext, npass);

			glEnable(GL_DEPTH_TEST);
			m_pMainWorldPtr->getRenderer()->setRenderPassType(gxRenderer::RENDER_NORMAL);
			GetNvPmApi()->BeginObject(nvProfiler::g_hNVPMContext, 0);
			monoWrapper::mono_engine_render(m_pMainWorldPtr, NULL);
			GetNvPmApi()->EndObject(nvProfiler::g_hNVPMContext, 0);

			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE);

			std::vector<gxLight*>* lightList = m_pMainWorldPtr->getLightList();
			for(int x=0;x<lightList->size();x++)
			{
				gxLight* light = lightList->at(x);
				if(!light->isBaseFlag(object3d::eObject3dBaseFlag_Visible))
					continue;

				m_pMainWorldPtr->getRenderer()->setRenderPassType(gxRenderer::RENDER_LIGHTING_ONLY);
				//Note:- glDepthFunc(GL_LEQUAL); by default its GL_LEQUAL in engine so no need to change here
				GetNvPmApi()->BeginObject(nvProfiler::g_hNVPMContext, 1);
				monoWrapper::mono_engine_render(m_pMainWorldPtr, light);
				GetNvPmApi()->EndObject(nvProfiler::g_hNVPMContext, 1);
			}
			glDisable(GL_BLEND);

			GetNvPmApi()->EndPass(nvProfiler::g_hNVPMContext, npass);
		}
		GetNvPmApi()->EndExperiment(nvProfiler::g_hNVPMContext);
	}
	else
	{
		monoWrapper::mono_engine_render(m_pMainWorldPtr, NULL);
	}
#else
	monoWrapper::mono_engine_render(m_pMainWorldPtr, NULL);
#endif

	CHECK_GL_ERROR(drawGrid());
	CHECK_GL_ERROR(drawSelectedObject());
	CHECK_GL_ERROR(drawOctree());
	m_cMultiPassFBO.UnBindFBO();
}

void gearSceneWorldEditor::drawGrid()
{
	//grid
	if(m_pTBGridView->isButtonPressed())
	{
		gxHWShader* shader = engine_getHWShaderManager()->GetHWShader(HW_BUILTIN_DEFAULT_SHADER_ONLY_DIFFUSE);
		shader->enableProgram();

		matrix4x4f gridOffsetTM;
		gridOffsetTM.setPosition(0, 0, -1);
		const float* u_mvp_m4x4=(*m_pMainWorldPtr->getRenderer()->getViewProjectionMatrix() * gridOffsetTM).getOGLMatrix();
		shader->sendUniformTMfv("u_mvp_m4x4", u_mvp_m4x4, false, 4);

		vector4f diffuseClr(0.2f, 0.2f, 0.2f, 1.0f);
		shader->sendUniform4fv("u_diffuse_v4", &diffuseClr.x);

		glEnableVertexAttribArray(shader->getAttribLoc("a_vertex_coord_v4"));

		glVertexAttribPointer(shader->getAttribLoc("a_vertex_coord_v4"), 2, GL_FLOAT, GL_FALSE, 0, &m_cGridOnYAxis[0].x);
		glDrawArrays(GL_LINES, 0, 90*2);
		glVertexAttribPointer(shader->getAttribLoc("a_vertex_coord_v4"), 2, GL_FLOAT, GL_FALSE, 0, &m_cGridOnXAxis[0].x);
		glDrawArrays(GL_LINES, 0, 90*2);

		diffuseClr.set(0.1f, 0.1f, 0.1f, 1.0f);
		shader->sendUniform4fv("u_diffuse_v4", &diffuseClr.x);

		glVertexAttribPointer(shader->getAttribLoc("a_vertex_coord_v4"), 2, GL_FLOAT, GL_FALSE, 0, &m_cThickGridOnYAxis[0].x);
		glDrawArrays(GL_LINES, 0, 9*2);
		glVertexAttribPointer(shader->getAttribLoc("a_vertex_coord_v4"), 2, GL_FLOAT, GL_FALSE, 0, &m_cThickGridOnXAxis[0].x);
		glDrawArrays(GL_LINES, 0, 9*2);

		glVertexAttribPointer(shader->getAttribLoc("a_vertex_coord_v4"), 2, GL_FLOAT, GL_FALSE, 0, &m_cGridOuterBox[0].x);
		glDrawArrays(GL_LINE_LOOP, 0, 4);

		glDisableVertexAttribArray(shader->getAttribLoc("a_vertex_coord_v4"));
		shader->disableProgram();
	}
	//
}

void gearSceneWorldEditor::drawCameraFrustum(gxCamera* camera, gxHWShader* shader)
{
	gxFrustumf* frustum=camera->getFrustum();

	float lineAry[]={
		frustum->m_cFrustumVert[0].x, frustum->m_cFrustumVert[0].y, frustum->m_cFrustumVert[0].z,		//0	near
		frustum->m_cFrustumVert[1].x, frustum->m_cFrustumVert[1].y, frustum->m_cFrustumVert[1].z,		//1
		frustum->m_cFrustumVert[2].x, frustum->m_cFrustumVert[2].y, frustum->m_cFrustumVert[2].z,		//2
		frustum->m_cFrustumVert[3].x, frustum->m_cFrustumVert[3].y, frustum->m_cFrustumVert[3].z,		//3

		frustum->m_cFrustumVert[4].x, frustum->m_cFrustumVert[4].y, frustum->m_cFrustumVert[4].z,		//4	far
		frustum->m_cFrustumVert[5].x, frustum->m_cFrustumVert[5].y, frustum->m_cFrustumVert[5].z,		//5
		frustum->m_cFrustumVert[6].x, frustum->m_cFrustumVert[6].y, frustum->m_cFrustumVert[6].z,		//6
		frustum->m_cFrustumVert[7].x, frustum->m_cFrustumVert[7].y, frustum->m_cFrustumVert[7].z,		//7

		frustum->m_cFrustumVert[0].x, frustum->m_cFrustumVert[0].y, frustum->m_cFrustumVert[0].z,		//0	near
		frustum->m_cFrustumVert[4].x, frustum->m_cFrustumVert[4].y, frustum->m_cFrustumVert[4].z,		//4	far

		frustum->m_cFrustumVert[1].x, frustum->m_cFrustumVert[1].y, frustum->m_cFrustumVert[1].z,		//1
		frustum->m_cFrustumVert[5].x, frustum->m_cFrustumVert[5].y, frustum->m_cFrustumVert[5].z,		//5

		frustum->m_cFrustumVert[2].x, frustum->m_cFrustumVert[2].y, frustum->m_cFrustumVert[2].z,		//2
		frustum->m_cFrustumVert[6].x, frustum->m_cFrustumVert[6].y, frustum->m_cFrustumVert[6].z,		//6

		frustum->m_cFrustumVert[3].x, frustum->m_cFrustumVert[3].y, frustum->m_cFrustumVert[3].z,		//3
		frustum->m_cFrustumVert[7].x, frustum->m_cFrustumVert[7].y, frustum->m_cFrustumVert[7].z,		//7
	};

	unsigned int mode=GL_LINE_LOOP;

#if defined (USE_ProgrammablePipeLine)
        glEnableVertexAttribArray(shader->getAttribLoc("a_vertex_coord_v4"));
		//near
		glVertexAttribPointer(shader->getAttribLoc("a_vertex_coord_v4"), 3, GL_FLOAT, GL_FALSE, 0, lineAry);
		glDrawArrays(mode, 0, 4);

		//far
		glVertexAttribPointer(shader->getAttribLoc("a_vertex_coord_v4"), 3, GL_FLOAT, GL_FALSE, 0, &lineAry[12]);
		glDrawArrays(mode, 0, 4);

		//side lines
		glVertexAttribPointer(shader->getAttribLoc("a_vertex_coord_v4"), 3, GL_FLOAT, GL_FALSE, 0, &lineAry[24+6*0]);
		glDrawArrays(mode, 0, 2);

		//side lines
		glVertexAttribPointer(shader->getAttribLoc("a_vertex_coord_v4"), 3, GL_FLOAT, GL_FALSE, 0, &lineAry[24+6*1]);
		glDrawArrays(mode, 0, 2);

		//side lines
		glVertexAttribPointer(shader->getAttribLoc("a_vertex_coord_v4"), 3, GL_FLOAT, GL_FALSE, 0, &lineAry[24+6*2]);
		glDrawArrays(mode, 0, 2);

		//side lines
		glVertexAttribPointer(shader->getAttribLoc("a_vertex_coord_v4"), 3, GL_FLOAT, GL_FALSE, 0, &lineAry[24+6*3]);
		glDrawArrays(mode, 0, 2);

		glDisableVertexAttribArray(shader->getAttribLoc("a_vertex_coord_v4"));
#endif
}

void gearSceneWorldEditor::drawSelectedObject()
{
	//lights
	std::vector<gxLight*>* lightList = m_pMainWorldPtr->getLightList();
	if(lightList->size() && m_pMainWorldPtr->getActiveCamera())
	{
		gxHWShader* shader = engine_getHWShaderManager()->GetHWShader(HW_BUILTIN_DEFAULT_SHADER_ONLY_GUI_SHADER);
		shader->enableProgram();
		for(int x=0;x<lightList->size();x++)
		{
			gxLight* light=lightList->at(x);
			float distance_frm_cam=(light->getWorldMatrix()->getPosition()-m_pMainWorldPtr->getActiveCamera()->getWorldMatrix()->getPosition()).length();
			if(distance_frm_cam<500.0f)
				distance_frm_cam=1.0f;
			else
				distance_frm_cam/=500.0f;

			m_cLightBillBoardSprite.setClip(277, 383, 18, 18);
			m_cLightBillBoardSprite.setDirection(&m_pMainWorldPtr->getActiveCamera()->getYAxis(), &m_pMainWorldPtr->getActiveCamera()->getZAxis()/**/);
			m_cLightBillBoardSprite.scale(distance_frm_cam*0.5f);
			const float* u_mvp_m4x4= (*m_pMainWorldPtr->getRenderer()->getViewProjectionMatrix() * (*light->getWorldMatrix() * m_cLightBillBoardSprite)).getMatrix();
			shader->sendUniformTMfv("u_mvp_m4x4", u_mvp_m4x4, false, 4);

			glDisable(GL_CULL_FACE);
			m_cLightBillBoardSprite.draw(shader);
			glEnable(GL_CULL_FACE);
		}
		shader->disableProgram();
	}
	//

	if(m_pSelectedObj && m_pMainWorldPtr->getActiveCamera())
	{
		gxHWShader* shader = engine_getHWShaderManager()->GetHWShader(HW_BUILTIN_DEFAULT_SHADER_ONLY_DIFFUSE_WITH_COLOR_PTR);
		shader->enableProgram();

		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHTING);
		
		float distance_frm_cam=(m_pSelectedObj->getWorldMatrix()->getPosition()-m_pMainWorldPtr->getActiveCamera()->getWorldMatrix()->getPosition()).length();
		if(distance_frm_cam<12.0f)
			distance_frm_cam=1.0f;
		else
			distance_frm_cam/=12.0f;

		//matrix4x4f localTM(*m_pSelectedObj->getWorldMatrix());
		//vector3f localScale(localTM.getScale());
		//localScale.normalize();
		//localTM.setScale(localScale);
		//glPushMatrix();
		const float* u_mvp_m4x4_local=NULL;
		const float* u_mvp_m4x4_world=NULL;
		//if(m_bTransformThroughLocalAxis)
		{
			u_mvp_m4x4_local = (*m_pMainWorldPtr->getRenderer()->getViewProjectionMatrix() * *m_pSelectedObj->getWorldMatrix()).getMatrix();
		}
		//else
		{
			matrix4x4f globalAxisTM;
			globalAxisTM.setPosition(m_pSelectedObj->getWorldMatrix()->getPosition());
			u_mvp_m4x4_world = (*m_pMainWorldPtr->getRenderer()->getViewProjectionMatrix() * globalAxisTM).getMatrix();
		}

		glEnable(GL_COLOR_MATERIAL);
		if(m_bTransformThroughLocalAxis)
			shader->sendUniformTMfv("u_mvp_m4x4", u_mvp_m4x4_local, false, 4);
		else
			shader->sendUniformTMfv("u_mvp_m4x4", u_mvp_m4x4_world, false, 4);
		geUtil::drawGizmo(distance_frm_cam, shader, m_iAxisSelected);

		shader->disableProgram();

		shader = engine_getHWShaderManager()->GetHWShader(HW_BUILTIN_DEFAULT_SHADER_ONLY_DIFFUSE);
		shader->enableProgram();

		shader->sendUniformTMfv("u_mvp_m4x4", u_mvp_m4x4_local, false, 4);
		//glColor4f(0.25f, 0.4f, 0.62f, 1);
#if USE_BULLET
		if(m_pSelectedObj->getRigidBody())
			shader->sendUniform4f("u_diffuse_v4", 0.25f, 0.0f, 0.62f, 1.0f);
		else
#endif
			shader->sendUniform4f("u_diffuse_v4", 0.25f, 0.4f, 0.62f, 1.0f);
		if(m_pTBShowOOBB->isButtonPressed())
		{
			m_pSelectedObj->getOOBB().draw(shader);
		}
		else
		{
			shader->sendUniformTMfv("u_mvp_m4x4", m_pMainWorldPtr->getRenderer()->getViewProjectionMatrix()->getMatrix(), false, 4);
			shader->sendUniform4f("u_diffuse_v4", 0.6f, 0.4f, 0.62f, 1.0f);
			m_pSelectedObj->getAABB().draw(shader);
			//m_pMainWorldPtr->getAABB().draw(shader);
		}

		if(m_pSelectedObj->isBaseFlag(object3d::eObject3dBaseFlag_Visible) && m_pSelectedObj->getID()==OBJECT3D_CAMERA_STRUCT)
		{
			shader->sendUniformTMfv("u_mvp_m4x4", u_mvp_m4x4_local, false, 4);
			shader->sendUniform4f("u_diffuse_v4", 0.6f, 0.6f, 0.6f, 1.0f);
			drawCameraFrustum((gxCamera*)m_pSelectedObj, shader);
		}

		glDisable(GL_COLOR_MATERIAL);

		shader->disableProgram();

		//glPopMatrix();

		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHTING);
	}
}

void gearSceneWorldEditor::drawOctree()
{
	if(m_pTBShowOctree->isButtonPressed() && m_pMainWorldPtr->getOctree())
	{
		gxHWShader* shader = engine_getHWShaderManager()->GetHWShader(HW_BUILTIN_DEFAULT_SHADER_ONLY_DIFFUSE);
		shader->enableProgram();

		shader->sendUniformTMfv("u_mvp_m4x4", m_pMainWorldPtr->getRenderer()->getViewProjectionMatrix()->getMatrix(), false, 4);
		m_pMainWorldPtr->getOctree()->drawOctree(m_pMainWorldPtr->getOctree()->getRoot(), shader);
		
		shader->disableProgram();
	}
}

void gearSceneWorldEditor::drawStats()
{
	//STATS
	glViewport(m_cPos.x+getIamOnLayout()->getPos().x, (m_pRenderer->getViewPortSz().y)-(m_cPos.y+getIamOnLayout()->getPos().y+m_cSize.y), m_cSize.x, m_cSize.y-getTopMarginOffsetHeight());	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho((int)0, (int)(m_cSize.x), (int)(m_cSize.y-getTopMarginOffsetHeight()), (int)0, -100, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushMatrix();
		glTranslatef(0, 0, -1);

		glDisable(GL_DEPTH_TEST);
#if defined USE_FBO
		drawFBO(m_cMultiPassFBO.getFBOTextureBuffer(0), 0.0f, -getTopMarginOffsetHeight(), m_cSize.x, m_cSize.y);
		drawFBO(m_cShadowMapFBO.getFBOTextureDepthShadowBuffer(), m_cSize.x-210, -(getTopMarginOffsetHeight())+m_cSize.y-210, 200, 200);
#endif

		char buffer[128];
		sprintf(buffer, "FPS : %3.2f", Timer::getFPS());
		geGUIManager::g_pFontArial10_84Ptr->drawString(buffer, 0, geGUIManager::g_pFontArial10_84Ptr->getLineHeight(), m_cSize.x);
		sprintf(buffer, "OpenGL %d.%d", rendererBase::g_iOGLMajorVersion, rendererBase::g_iOGLMinorVersion);
		geGUIManager::g_pFontArial10_84Ptr->drawString(buffer, 0, geGUIManager::g_pFontArial10_84Ptr->getLineHeight()*3, m_cSize.x);
		sprintf(buffer, "GLSL %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
		geGUIManager::g_pFontArial10_84Ptr->drawString(buffer, 0, geGUIManager::g_pFontArial10_84Ptr->getLineHeight()*4, m_cSize.x);
		sprintf(buffer, "TimeScale : %1.2f", m_pHorizontalSlider_TimeScale->getSliderValue());
		geGUIManager::g_pFontArial10_84Ptr->drawString(buffer, 0, geGUIManager::g_pFontArial10_84Ptr->getLineHeight()*5, m_cSize.x);
		sprintf(buffer, "nTris : %d", m_pMainWorldPtr->getRenderer()->m_nTrisRendered);
		geGUIManager::g_pFontArial10_84Ptr->drawString(buffer, 0, geGUIManager::g_pFontArial10_84Ptr->getLineHeight()*6, m_cSize.x);
		sprintf(buffer, "nDrawCalls : %d", m_pMainWorldPtr->getRenderer()->m_nDrawCalls);
		geGUIManager::g_pFontArial10_84Ptr->drawString(buffer, 0, geGUIManager::g_pFontArial10_84Ptr->getLineHeight()*7, m_cSize.x);

		sprintf(buffer, "Total Material : %d", monoWrapper::mono_engine_getWorld(0)->getMaterialList()->size());
		geGUIManager::g_pFontArial10_84Ptr->drawString(buffer, 0, geGUIManager::g_pFontArial10_84Ptr->getLineHeight()*8, m_cSize.x);
		sprintf(buffer, "Total Animation : %d", monoWrapper::mono_engine_getWorld(0)->getAnimationSetList()->size());
		geGUIManager::g_pFontArial10_84Ptr->drawString(buffer, 0, geGUIManager::g_pFontArial10_84Ptr->getLineHeight()*9, m_cSize.x);
		//int last_gl_err=glGetError();
		//if(last_gl_err!=GL_NO_ERROR)
		//	m_iLastGLError=last_gl_err;
		sprintf(buffer, "glGetError : 0x%x", m_iLastGLError);
		geGUIManager::g_pFontArial10_84Ptr->drawString(buffer, 0, geGUIManager::g_pFontArial10_84Ptr->getLineHeight()*10, m_cSize.x);

		sprintf(buffer, "Total Layer Objects : %d", m_pMainWorldPtr->getLayerManager()->getTotalLayerObject());
		geGUIManager::g_pFontArial10_84Ptr->drawString(buffer, 0, geGUIManager::g_pFontArial10_84Ptr->getLineHeight()*11, m_cSize.x);

		geGUIBase* selectedNodeInHirarchy=EditorApp::getSceneHierarchy()->getSelectedTreeNode();
		if(selectedNodeInHirarchy)
		{
			object3d* obj=(object3d*)selectedNodeInHirarchy->getUserData();
			if(obj && obj->getAnimationController())
			{
				sprintf(buffer, "Current Frame : %4.2f", obj->getAnimationController()->getCurrentFrame());
				geGUIManager::g_pFontArial10_84Ptr->drawString(buffer, 0, 0+geGUIManager::g_pFontArial10_84Ptr->getLineHeight()*12, m_cSize.x);
			}
		}

#if USE_NVPROFILER
		nvProfiler::SampleAndRenderStats();
#endif

		glEnable(GL_DEPTH_TEST);

		gxWorld* world = monoWrapper::mono_engine_getWorld(0);
		if(world->getActiveCamera())
		{
			matrix4x4f cameramatrix(*world->getActiveCamera()->getInverseTMViewMatrix());
			cameramatrix.setPosition(0, 0, 0);
			cameramatrix.setZAxis(-cameramatrix.getZAxis());

			glPushMatrix();
				glTranslatef(m_cSize.x-50, 50, 0);
				glMultMatrixf(cameramatrix.getMatrix());
				glEnable(GL_COLOR_MATERIAL);
				glColor3f(1, 1, 1);
				glutSolidCube(10);
				geUtil::drawGizmoCones(60);
				glDisable(GL_COLOR_MATERIAL);
			glPopMatrix();
		}
	glPopMatrix();
	//
}

void gearSceneWorldEditor::preWorldRender()
{

}

void gearSceneWorldEditor::postWorldRender()
{
	//glBegin(GL_LINES);
	//glVertex3fv(&minV.x);
	//glVertex3fv(&maxV.x);
	//glEnd();

	//glDisable(GL_DEPTH_TEST);
	//glPushMatrix();
	//glTranslatef(i1.x, i1.y, i1.z);
	////glColor4f(1, 0 , 0, 1);
	//glutWireSphere(0.1f, 3, 3);
	//glPopMatrix();
	//glPushMatrix();
	//glTranslatef(testSpehere.x, testSpehere.y, testSpehere.z);
	//glColor4f(1, 0 , 0, 1);
	//glutWireSphere(0.1f, 3, 3);
	//glPopMatrix();
	//glEnable(GL_DEPTH_TEST);
}

void gearSceneWorldEditor::startFollowCam()
{
	if(m_pPlayButton->isButtonPressed())
	{
		if(!m_pPauseButton->isButtonPressed())
			return;
	}
	if(m_pSelectedObj==NULL) return;
	if(m_pSelectedObj->getID()==OBJECT3D_CAMERA) return;

	m_bStopFollowCam=false;
}

void gearSceneWorldEditor::followObject(float dt, object3d* chasedObj)
{
	if(chasedObj && chasedObj->getID()==OBJECT3D_CAMERA)
	{
		m_bStopFollowCam=true;
		return;
	}
	if(dt>0.1f || m_bStopFollowCam) return;
	if(chasedObj==NULL) return;
	gxWorld* world = monoWrapper::mono_engine_getWorld(0);
	Camera* cam=world->getActiveCamera();
	matrix4x4f* chasingObj=(matrix4x4f*)cam;
	vector3f	eyeOff;
	vector3f	chasedObjectCenter(chasedObj->getAABB().getCenter());
	float		speed=10.0f;

	vector3f direction(cam->getWorldMatrix()->getPosition()-chasedObjectCenter);
	direction.normalize();
	eyeOff = direction*(chasedObj->getAABB().getLongestAxis()*0.5f + cam->getNear())*4.0f;

	vector3f    transformedEye(chasedObjectCenter + eyeOff);
	vector3f    transformedLookAt(chasedObjectCenter);
	
	vector3f    chasingObjPos(cam->getWorldMatrix()->getPosition());
    vector3f    chasedObjPos(chasedObjectCenter);
    vector3f    lenV(transformedEye-chasingObjPos);
    float		len=lenV.length();
	
    if(len<=0.01f)
	{
		m_bStopFollowCam=true;
		return;
	}

    if(len>4000.0f)
    {
        float factor=4000.0f/len;
        lenV=lenV*factor;
    }
	
    vector3f    updatedPos(chasingObjPos+lenV*(speed*dt));
    vector3f forward(updatedPos-transformedLookAt);
    forward.normalize();
    vector3f up(0, 0, 1);
    vector3f left(up.cross(forward));
    left.normalize();
    up=forward.cross(left);
    up.normalize();
	
	chasingObj->setXAxis(left);
	chasingObj->setYAxis(up);
	chasingObj->setZAxis(forward);
	chasingObj->setPosition(updatedPos);
	
	cam->updateCamera();
}

void gearSceneWorldEditor::onSize(float cx, float cy, int flag)
{
#if defined USE_FBO
	if(cx>1.0f && cy>1.0f)
	{
		m_cMultiPassFBO.ReInitFBO(cx, cy);
		m_cMultiPassFBO.CreateDepthBuffer();
		m_cMultiPassFBO.AttachDepthBuffer();
		m_cMultiPassFBO.CreateTextureBuffer();
		m_cMultiPassFBO.AttachTextureBuffer(0);
		m_cMultiPassFBO.UnBindFBO();
		
#if USE_NVPROFILER
		gTraceDisplay_DisplayW=cx;
		gTraceDisplay_DisplayH=cy;
#endif
	}
#endif

	geWindow::onSize(cx, cy, flag);
}

static bool intersectionOfLineSegmentAndPlane(vector3f p1, vector3f p2, vector3f planeNormal, vector3f pointOnPlane, float& u)
{
	vector3f p3_p1(pointOnPlane-p1);
	vector3f p2_p1(p2-p1);

	float n = planeNormal.dot(p3_p1);
	float d = planeNormal.dot(p2_p1);

	if(abs(d)<0.00000001f)
		return false;		//line parallel to plane

	u=n/d;
	return true;
}

bool gearSceneWorldEditor::onMouseLButtonDown(float x, float y, int nFlag)
{
	if(!isPointInsideClientArea(x, y))
	{
		m_iAxisSelected=-1;
		return true;
	}
	//monoWrapper::mono_engine_mouseLButtonDown(m_pMainWorldPtr, x, y, nFlag);
	if(!m_pMainWorldPtr || !m_pMainWorldPtr->getActiveCamera()) return true;

	bool bTranslateGizmo=m_pTranslateGizmo->isButtonPressed();
	bool bRotateGizmo=m_pRotateGizmo->isButtonPressed();
	bool bScaleGizmo=m_pScaleGizmo->isButtonPressed();

	if(!bTranslateGizmo && !bRotateGizmo && !bScaleGizmo)
		return true;

	GLdouble viewTM[16];
	GLdouble projectionTM[16];
	for(int m=0;m<16;m++)
	{
		viewTM[m]=m_pMainWorldPtr->getActiveCamera()->getViewMatrix()->getMatrix()[m];
		projectionTM[m]=m_pMainWorldPtr->getActiveCamera()->getProjectionMatrix()->getMatrix()[m];
	}

	GLdouble x1, y1, z1, depth;
	GLdouble x2, y2, z2;
	//CHECK_GL_ERROR(glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_DOUBLE, &depth));
	z1=z2=depth=0.0f;

	gxRectf viewPortRect=m_pMainWorldPtr->getRenderer()->getViewPortRect();
	GLint viewport[4]={viewPortRect.m_pos.x, viewPortRect.m_pos.y, viewPortRect.m_size.x, viewPortRect.m_size.y}; 
	//float ptY=Scene::getCommonData()->getScreenHeight();
	CHECK_GL_ERROR(gluUnProject(x, viewPortRect.m_size.y - y, 0, viewTM, projectionTM, viewport, &x1, &y1, &z1));
	CHECK_GL_ERROR(gluUnProject(x, viewPortRect.m_size.y - y, 1, viewTM, projectionTM, viewport, &x2, &y2, &z2));
	
	minV.set(x1, y1, z1);
	maxV.set(x2, y2, z2);

	m_iAxisSelected=0;
	if(m_pSelectedObj)
	{
		float distance_frm_cam=(m_pSelectedObj->getWorldMatrix()->getPosition()-m_pMainWorldPtr->getActiveCamera()->getWorldMatrix()->getPosition()).length();
		if(distance_frm_cam<12.0f)
			distance_frm_cam=1.0f;
		else
			distance_frm_cam/=12.0f;

		//x-gizmo
		vector3f gizmo_origin(m_pSelectedObj->getWorldMatrix()->getPosition());
		vector3f xgizmo_max(distance_frm_cam, 0, 0);
		vector3f ygizmo_max(0, distance_frm_cam, 0);
		vector3f zgizmo_max(0, 0, distance_frm_cam);

		if(m_bTransformThroughLocalAxis)
		{
			xgizmo_max=*m_pSelectedObj->getWorldMatrix() * xgizmo_max ;
			ygizmo_max=*m_pSelectedObj->getWorldMatrix() * ygizmo_max ;
			zgizmo_max=*m_pSelectedObj->getWorldMatrix() * zgizmo_max ;
		}
		else
		{
			xgizmo_max=xgizmo_max+gizmo_origin;
			ygizmo_max=ygizmo_max+gizmo_origin;
			zgizmo_max=zgizmo_max+gizmo_origin;
		}

		float ua, ub;
		//x-axis
		if(gxUtil::twoLineSegmentIntersection(minV, maxV, gizmo_origin, xgizmo_max, i1, i2, ua, ub))
		{
			if(ub>=0.0f && ub<=1.0f)
			{
				if((i1-i2).length()<=distance_frm_cam*0.06f)
				{
					testSpehere=i2;
					m_iAxisSelected=1;
					float pu;
					vector3f plane_normal((m_bTransformThroughLocalAxis)?m_pSelectedObj->getWorldMatrix()->getZAxis():vector3f(0, 0, 1));
					if(intersectionOfLineSegmentAndPlane(minV, maxV, plane_normal, m_pSelectedObj->getWorldMatrix()->getPosition(), pu))
					{
						//vector3f 
						m_cMousePrevPosInWorld = minV+(maxV-minV)*pu;
					}
				}
			}
		}
		if(gxUtil::twoLineSegmentIntersection(minV, maxV, gizmo_origin, ygizmo_max, i1, i2, ua, ub))
		{
			if(ub>=0.0f && ub<=1.0f)
			{
				if((i1-i2).length()<=distance_frm_cam*0.06f)
				{
					testSpehere=i2;
					m_iAxisSelected=2;
					float pu;
					vector3f plane_normal((m_bTransformThroughLocalAxis)?m_pSelectedObj->getWorldMatrix()->getZAxis():vector3f(0, 0, 1));
					if(intersectionOfLineSegmentAndPlane(minV, maxV, plane_normal, m_pSelectedObj->getWorldMatrix()->getPosition(), pu))
					{
						//vector3f 
						m_cMousePrevPosInWorld = minV+(maxV-minV)*pu;
					}
				}
			}
		}
		if(gxUtil::twoLineSegmentIntersection(minV, maxV, gizmo_origin, zgizmo_max, i1, i2, ua, ub))
		{
			if(ub>=0.0f && ub<=1.0f)
			{
				if((i1-i2).length()<=distance_frm_cam*0.06f)
				{
					testSpehere=i2;
					m_iAxisSelected=3;
					float pu;
					vector3f plane_normal((m_bTransformThroughLocalAxis)?m_pSelectedObj->getWorldMatrix()->getYAxis():vector3f(0, 1, 0));
					if(intersectionOfLineSegmentAndPlane(minV, maxV, plane_normal, m_pSelectedObj->getWorldMatrix()->getPosition(), pu))
					{
						//vector3f 
						m_cMousePrevPosInWorld = minV+(maxV-minV)*pu;
					}
				}
			}
		}
	}

	m_cMousePrevPos.set(x, y);
	return true;
}

bool gearSceneWorldEditor::onMouseLButtonUp(float x, float y, int nFlag)
{
	if(!isPointInsideClientArea(x, y))
	{
		return true;
	}

	if(m_iAxisSelected>0)
		EditorApp::getSceneHierarchy()->recreateOctree();

	m_iAxisSelected=-1;
	monoWrapper::mono_engine_mouseLButtonUp(m_pMainWorldPtr, x, y, nFlag);
	m_cMousePrevPos.set(x, y);
	return true;
}

bool gearSceneWorldEditor::onMouseRButtonDown(float x, float y, int nFlag)
{
	if(!isPointInsideClientArea(x, y))
	{
		m_iAxisSelected=-1;
		return true;
	}
	monoWrapper::mono_engine_mouseRButtonDown(m_pMainWorldPtr, x, y, nFlag);
	m_cMousePrevPos.set(x, y);
	return true;
}

void gearSceneWorldEditor::onMouseRButtonUp(float x, float y, int nFlag)
{
	if(!isPointInsideClientArea(x, y))
	{
		m_iAxisSelected=-1;
		return;
	}
	monoWrapper::mono_engine_mouseRButtonUp(m_pMainWorldPtr, x, y, nFlag);
	m_cMousePrevPos.set(x, y);
}

bool gearSceneWorldEditor::onMouseMove(float x, float y, int flag)
{
	if(!isPointInsideClientArea(x, y)) return false;
	//monoWrapper::mono_engine_mouseMove(m_pMainWorldPtr, x, y, flag);
	Camera* camera=m_pMainWorldPtr->getActiveCamera();
	
	if(!camera) return false;

	float dx	= (x-m_cMousePrevPos.x);
	float dy	= (y-m_cMousePrevPos.y);

	if(flag&MK_MBUTTON)
	{
		float d=camera->getPosition().length();
		if(flag&MK_SHIFT)
			camera->updateLocalPositionf(-(d/5000.0f)*dx, (d/5000.0f)*dy, 0);
		else
			camera->updateLocalPositionf(-(d/500.0f)*dx, (d/500.0f)*dy, 0);
	}
	else if(flag&MK_RBUTTON /*&& flag&MK_CONTROL*/)
	{

		vector3f aUP(0, 0, 1);
		//aUP=camera->getYAxis();
		vector3f aVect(0, 0, 0);
		if(m_pSelectedObj)
			aVect=m_pSelectedObj->getWorldMatrix()->getPosition();	//can modify this later to rotate around mesh center

		float factor=1.0f;
		if(flag&MK_SHIFT)
			factor=0.01f;
		camera->rotateArb(-0.5f*dx*factor, &aUP.x, aVect);
		vector3f left=camera->getXAxis();
		camera->rotateArb(-0.5f*dy*factor, &left.x, aVect);
	}
	else if(flag&MK_LBUTTON)
	{
		bool bTranslateGizmo=m_pTranslateGizmo->isButtonPressed();
		bool bRotateGizmo=m_pRotateGizmo->isButtonPressed();
		bool bScaleGizmo=m_pScaleGizmo->isButtonPressed();

		if((bTranslateGizmo || bRotateGizmo || bScaleGizmo))
		{
			if(m_pSelectedObj && m_iAxisSelected>0)
			{
				float d=camera->getPosition().length();
				GLdouble viewTM[16];
				GLdouble projectionTM[16];
				for(int m=0;m<16;m++)
				{
					viewTM[m]=m_pMainWorldPtr->getActiveCamera()->getViewMatrix()->getMatrix()[m];
					projectionTM[m]=m_pMainWorldPtr->getActiveCamera()->getProjectionMatrix()->getMatrix()[m];
				}

				GLdouble x1, y1, z1, depth;
				GLdouble x2, y2, z2;
				//CHECK_GL_ERROR(glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_DOUBLE, &depth));
				z1=z2=depth=0.0f;

				gxRectf viewPortRect=m_pMainWorldPtr->getRenderer()->getViewPortRect();
				GLint viewport[4]={viewPortRect.m_pos.x, viewPortRect.m_pos.y, viewPortRect.m_size.x, viewPortRect.m_size.y}; 
				//float ptY=Scene::getCommonData()->getScreenHeight();
				CHECK_GL_ERROR(gluUnProject(x, viewPortRect.m_size.y - y, 0, viewTM, projectionTM, viewport, &x1, &y1, &z1));
				CHECK_GL_ERROR(gluUnProject(x, viewPortRect.m_size.y - y, 1, viewTM, projectionTM, viewport, &x2, &y2, &z2));

				minV.set(x1, y1, z1);
				maxV.set(x2, y2, z2);

				vector3f planeNormalArray[3]={

					(m_bTransformThroughLocalAxis)?m_pSelectedObj->getWorldMatrix()->getZAxis():vector3f(0, 0, 1),
					(m_bTransformThroughLocalAxis)?m_pSelectedObj->getWorldMatrix()->getZAxis():vector3f(0, 0, 1),
					(m_bTransformThroughLocalAxis)?m_pSelectedObj->getWorldMatrix()->getYAxis():vector3f(0, 1, 0)
				};

				float pu;
				if(intersectionOfLineSegmentAndPlane(minV, maxV, planeNormalArray[m_iAxisSelected-1], m_pSelectedObj->getWorldMatrix()->getPosition(), pu))
				{
					vector3f current_pos_in_world(minV+(maxV-minV)*pu);
					vector3f diff_in_world(current_pos_in_world-m_cMousePrevPosInWorld);
					vector3f transformed;
					transformed=(m_bTransformThroughLocalAxis)?(m_pSelectedObj->getWorldMatrix()->getInverse() % diff_in_world):m_pSelectedObj->getWorldMatrix()->getPosition() + diff_in_world;
					//transformed=m_pSelectedObj->getWorldMatrix()->getInverse() % diff_in_world;

					if(m_bTransformThroughLocalAxis)
					{
						if(bTranslateGizmo)
						{
							if(m_iAxisSelected==1)
								m_pSelectedObj->updateLocalPositionf(transformed.x, 0, 0);
							else if(m_iAxisSelected==2)
								m_pSelectedObj->updateLocalPositionf(0, transformed.y, 0);
							else if(m_iAxisSelected==3)
								m_pSelectedObj->updateLocalPositionf(0, 0, transformed.z);
						}
						else if(bRotateGizmo)
						{
							if(m_iAxisSelected==1)
								m_pSelectedObj->rotateLocalXf(-0.5f*dy);
							else if(m_iAxisSelected==2)
								m_pSelectedObj->rotateLocalYf(-0.5f*dy);
							else if(m_iAxisSelected==3)
								m_pSelectedObj->rotateLocalZf(-0.5f*dx);
						}
						else if(bScaleGizmo)
						{
							if(m_iAxisSelected==1)
								m_pSelectedObj->scaleX(transformed.x);
							else if(m_iAxisSelected==2)
								m_pSelectedObj->scaleY(transformed.y);
							else if(m_iAxisSelected==3)
								m_pSelectedObj->scaleZ(transformed.z);
						}
					}
					else
					{
						vector3f right(1, 0, 0);
						right=right*diff_in_world.x;
						if(bTranslateGizmo)
						{
							if(m_iAxisSelected==1)
								m_pSelectedObj->updatePositionf(right.x, 0, 0);
							else if(m_iAxisSelected==2)
								m_pSelectedObj->updatePositionf(0, diff_in_world.y, 0);
							else if(m_iAxisSelected==3)
								m_pSelectedObj->updatePositionf(0, 0, diff_in_world.z);
						}
					}

					if(bTranslateGizmo || bRotateGizmo || bScaleGizmo)
						m_pMainWorldPtr->transformationChangedf();

					m_cMousePrevPosInWorld = current_pos_in_world;
				}
			}
		}
	}

	//DEBUG_PRINT("%f, %f\n", delta.x, delta.y);
	m_cMousePrevPos.set(x, y);

	return true;
}

void gearSceneWorldEditor::onMouseWheel(int zDelta, int x, int y, int flag)
{
	if(!isPointInsideClientArea(x, y)) return;

	m_bStopFollowCam=true;
	//monoWrapper::mono_engine_mouseWheel(m_pMainWorldPtr, zDelta, x, y, flag);
	Camera* camera=m_pMainWorldPtr->getActiveCamera();
	if(!camera)
		return;

	int dir = (zDelta<0)?1:-1;
	vector3f aCamForwardDir(camera->getZAxis());
	float d=camera->getPosition().length();
	float factor=20.0f;
	if(flag&MK_SHIFT)
		factor=500.0f;
	aCamForwardDir.x=aCamForwardDir.x*(d/factor)*(dir);
	aCamForwardDir.y=aCamForwardDir.y*(d/factor)*(dir);
	aCamForwardDir.z=aCamForwardDir.z*(d/factor)*(dir);
	camera->updatePositionf(aCamForwardDir.x, aCamForwardDir.y, aCamForwardDir.z);
}

void gearSceneWorldEditor::onButtonClicked(geGUIBase* btn)
{
	if(m_pLocalOrGlobalAxis==btn)
	{
		if(strcmp(m_pLocalOrGlobalAxis->getName(), "Local")==0)
		{
			m_pLocalOrGlobalAxis->setName("Global");
			m_bTransformThroughLocalAxis=false;
		}
		else
		{
			m_pLocalOrGlobalAxis->setName("Local");
			m_bTransformThroughLocalAxis=true;
		}
		m_pLocalOrGlobalAxis->buttonNormal(true);
	}
	else if(m_pPlayButton==btn)
	{
		if(m_pPlayButton->isButtonPressed())
		{
			EditorApp::getSceneConsole()->appendConsoleRunRootNode();
			monoWrapper::mono_game_start();
			m_bMonoGameInitialized=true;
		}
		else
		{
			m_pPauseButton->buttonNormal(true);
			m_bMonoGameInitialized=false;
		}
	}
	else if(m_pPauseButton==btn)
	{
		if(m_pPauseButton->isButtonPressed() && !m_pPlayButton->isButtonPressed())
		{
			//reset the button if not in play mode
			m_pPauseButton->buttonNormal(true);
		}
	}
	else if(m_pTranslateGizmo==btn)
	{
		if(m_pTranslateGizmo->isButtonPressed())
		{
			m_pRotateGizmo->buttonNormal(true);
			m_pScaleGizmo->buttonNormal(true);
		}
	}
	else if(m_pRotateGizmo==btn)
	{
		if(m_pRotateGizmo->isButtonPressed())
		{
			m_pTranslateGizmo->buttonNormal(true);
			m_pScaleGizmo->buttonNormal(true);
		}
	}
	else if(m_pScaleGizmo==btn)
	{
		if(m_pScaleGizmo->isButtonPressed())
		{
			m_pRotateGizmo->buttonNormal(true);
			m_pTranslateGizmo->buttonNormal(true);
		}
	}
}

void gearSceneWorldEditor::onSliderChange(geGUIBase* slider)
{
	if(slider==m_pHorizontalSlider_LightAmbient)
	{
		float val=0.1f+m_pHorizontalSlider_LightAmbient->getSliderValue()*0.13f;
		glClearColor(val, val, val, 1.0f);
	}
}

bool gearSceneWorldEditor::onKeyDown(int charValue, int flag)
{
#if USE_NVPROFILER
	if(charValue==32)
	{
		nvProfiler::nvDataProvider->removeAllCounters();

        // Add some SOL and bottleneck counters
        nvProfiler::nvDataProvider->add("SHD Bottleneck");
        nvProfiler::nvDataProvider->add("SHD SOL");
        nvProfiler::nvDataProvider->add("TEX Bottleneck");
        nvProfiler::nvDataProvider->add("TEX SOL");
        nvProfiler::nvDataProvider->add("ROP Bottleneck");
        nvProfiler::nvDataProvider->add("ROP SOL");
        nvProfiler::nvDataProvider->add("FB Bottleneck");
        nvProfiler::nvDataProvider->add("FB SOL");
		nvProfiler::g_bPerformanceAnalyze=true;
	}
#endif

	if(charValue==70)
	{
		startFollowCam();
	}

	monoWrapper::mono_game_onkeydown(charValue, flag);
	return geWindow::onKeyDown(charValue, flag);
}

bool gearSceneWorldEditor::onKeyUp(int charValue, int flag)
{
	monoWrapper::mono_game_onkeyup(charValue, flag);
	return geWindow::onKeyUp(charValue, flag);
}

void gearSceneWorldEditor::onCommand(int cmd)
{
	switch(cmd)
	{
	case ID_POSTPROCESSOR_BLURPROCESSOR:
		{
			m_bEnablePostProcessorBlur=!m_bEnablePostProcessorBlur;
			//EditorApp::getScenePropertyEditor()->populatePropertyOfBlurShader(engine_getHWShaderManager()->GetHWShader(HW_BUILTIN_DEFAULT_SHADER_ONLY_BLUR_SHADER));
		}
		break;
	}
}

void gearSceneWorldEditor::stopSimulation()
{
	m_pPlayButton->buttonNormal(true);
	m_pPauseButton->buttonNormal(true);
	m_bMonoGameInitialized=false;
}

//void drawRoundedRectangle(float x, float y, float cx, float cy, float deltaHeight)
//{
//	float rgb_top[4] ={0.3, 0.3, 0.3, 1.0f};
//	float rgb_bottom[4] ={0.3*0.45f, 0.3*0.45f, 0.3*0.45f, 1.0f};
//	float rgb_delta[4]={(rgb_bottom[0]-rgb_top[0]), (rgb_bottom[1]-rgb_top[1]), (rgb_bottom[2]-rgb_top[2]), (rgb_bottom[3]-rgb_top[3])};
//
//	const float horizontal_vertLst[8] =
//	{
//		cx,	deltaHeight,
//		0,	deltaHeight,
//		cx,	cy-deltaHeight,
//		0,	cy-deltaHeight,
//	};
//
//	const float horizontal_colorLst[16] =
//	{
//		rgb_top[0]+rgb_delta[0]*(deltaHeight/cy), rgb_top[1]+rgb_delta[1]*(deltaHeight/cy), rgb_top[2]+rgb_delta[2]*(deltaHeight/cy), rgb_top[3]+rgb_delta[3]*(deltaHeight/cy),
//		rgb_top[0]+rgb_delta[0]*(deltaHeight/cy), rgb_top[1]+rgb_delta[1]*(deltaHeight/cy), rgb_top[2]+rgb_delta[2]*(deltaHeight/cy), rgb_top[3]+rgb_delta[3]*(deltaHeight/cy),
//		rgb_bottom[0]-rgb_delta[0]*(deltaHeight/cy), rgb_bottom[1]-rgb_delta[1]*(deltaHeight/cy), rgb_bottom[2]-rgb_delta[2]*(deltaHeight/cy), rgb_bottom[3]-rgb_delta[3]*(deltaHeight/cy),
//		rgb_bottom[0]-rgb_delta[0]*(deltaHeight/cy), rgb_bottom[1]-rgb_delta[1]*(deltaHeight/cy), rgb_bottom[2]-rgb_delta[2]*(deltaHeight/cy), rgb_bottom[3]-rgb_delta[3]*(deltaHeight/cy)
//	};
//	
//	const float vertical_vertLst[8] =
//	{
//		cx-deltaHeight,	0,
//		deltaHeight,	0,
//		cx-deltaHeight,	cy,
//		deltaHeight,	cy,
//	};
//
//	const float vertical_colorLst[16] =
//	{
//		rgb_top[0], rgb_top[1], rgb_top[2], rgb_top[3],
//		rgb_top[0], rgb_top[1], rgb_top[2], rgb_top[3],
//		rgb_bottom[0], rgb_bottom[1], rgb_bottom[2], rgb_bottom[3],
//		rgb_bottom[0], rgb_bottom[1], rgb_bottom[2], rgb_bottom[3]
//	};
//
//	const int step=10;
//	float delta_angle=90/step;
//	const int szz=(2+step)*2;
//	const int cszz=(2+step)*4;
//	float rounded_left_top_vertList[szz];
//	float rounded_right_top_vertList[szz];
//	float rounded_right_bottom_vertList[szz];
//	float rounded_left_bottom_vertList[szz];
//
//	float rounded_left_top_colorList[cszz];
//	float rounded_right_top_colorList[cszz];
//	float rounded_right_bottom_colorList[cszz];
//	float rounded_left_bottom_colorList[cszz];
//
//	//left top
//	rounded_left_top_vertList[0]=deltaHeight;
//	rounded_left_top_vertList[1]=deltaHeight;
//	rounded_left_top_colorList[0]=rgb_top[0]+rgb_delta[0]*(rounded_left_top_vertList[1]/cy);
//	rounded_left_top_colorList[1]=rgb_top[1]+rgb_delta[1]*(rounded_left_top_vertList[1]/cy);
//	rounded_left_top_colorList[2]=rgb_top[2]+rgb_delta[2]*(rounded_left_top_vertList[1]/cy);
//	rounded_left_top_colorList[3]=rgb_top[3]+rgb_delta[3]*(rounded_left_top_vertList[1]/cy);
//
//	float angle=180;
//	for(int xx=step;xx>=0;xx--)
//	{
//		rounded_left_top_vertList[(xx+1)*2+0]=rounded_left_top_vertList[0]+deltaHeight*gxMath::COSF(angle);
//		rounded_left_top_vertList[(xx+1)*2+1]=rounded_left_top_vertList[1]+deltaHeight*gxMath::SINF(angle);
//
//		float color_height=rounded_left_top_vertList[(xx+1)*2+1];
//		rounded_left_top_colorList[(xx+1)*4+0]=rgb_top[0]+rgb_delta[0]*(color_height/cy);
//		rounded_left_top_colorList[(xx+1)*4+1]=rgb_top[1]+rgb_delta[1]*(color_height/cy);
//		rounded_left_top_colorList[(xx+1)*4+2]=rgb_top[2]+rgb_delta[2]*(color_height/cy);
//		rounded_left_top_colorList[(xx+1)*4+3]=rgb_top[3]+rgb_delta[3]*(color_height/cy);
//		angle+=delta_angle;
//	}
//
//	//right top
//	rounded_right_top_vertList[0]=cx-deltaHeight;
//	rounded_right_top_vertList[1]=deltaHeight;
//	rounded_right_top_colorList[0]=rgb_top[0]+rgb_delta[0]*(rounded_left_top_vertList[1]/cy);
//	rounded_right_top_colorList[1]=rgb_top[1]+rgb_delta[1]*(rounded_left_top_vertList[1]/cy);
//	rounded_right_top_colorList[2]=rgb_top[2]+rgb_delta[2]*(rounded_left_top_vertList[1]/cy);
//	rounded_right_top_colorList[3]=rgb_top[3]+rgb_delta[3]*(rounded_left_top_vertList[1]/cy);
//
//	angle=270;
//	for(int xx=step;xx>=0;xx--)
//	{
//		rounded_right_top_vertList[(xx+1)*2+0]=rounded_right_top_vertList[0]+deltaHeight*gxMath::COSF(angle);
//		rounded_right_top_vertList[(xx+1)*2+1]=rounded_right_top_vertList[1]+deltaHeight*gxMath::SINF(angle);
//
//		float color_height=rounded_right_top_vertList[(xx+1)*2+1];
//		rounded_right_top_colorList[(xx+1)*4+0]=rgb_top[0]+rgb_delta[0]*(color_height/cy);
//		rounded_right_top_colorList[(xx+1)*4+1]=rgb_top[1]+rgb_delta[1]*(color_height/cy);
//		rounded_right_top_colorList[(xx+1)*4+2]=rgb_top[2]+rgb_delta[2]*(color_height/cy);
//		rounded_right_top_colorList[(xx+1)*4+3]=rgb_top[3]+rgb_delta[3]*(color_height/cy);
//		angle+=delta_angle;
//	}
//
//	//right bottom
//	rounded_right_bottom_vertList[0]=cx-deltaHeight;
//	rounded_right_bottom_vertList[1]=cy-deltaHeight;
//	rounded_right_bottom_colorList[0]=rgb_top[0]+rgb_delta[0]*(rounded_right_bottom_vertList[1]/cy);
//	rounded_right_bottom_colorList[1]=rgb_top[1]+rgb_delta[1]*(rounded_right_bottom_vertList[1]/cy);
//	rounded_right_bottom_colorList[2]=rgb_top[2]+rgb_delta[2]*(rounded_right_bottom_vertList[1]/cy);
//	rounded_right_bottom_colorList[3]=rgb_top[3]+rgb_delta[3]*(rounded_right_bottom_vertList[1]/cy);
//
//	angle=0;
//	for(int xx=step;xx>=0;xx--)
//	{
//		rounded_right_bottom_vertList[(xx+1)*2+0]=rounded_right_bottom_vertList[0]+deltaHeight*gxMath::COSF(angle);
//		rounded_right_bottom_vertList[(xx+1)*2+1]=rounded_right_bottom_vertList[1]+deltaHeight*gxMath::SINF(angle);
//
//		float color_height=rounded_right_bottom_vertList[(xx+1)*2+1];
//		rounded_right_bottom_colorList[(xx+1)*4+0]=rgb_top[0]+rgb_delta[0]*(color_height/cy);
//		rounded_right_bottom_colorList[(xx+1)*4+1]=rgb_top[1]+rgb_delta[1]*(color_height/cy);
//		rounded_right_bottom_colorList[(xx+1)*4+2]=rgb_top[2]+rgb_delta[2]*(color_height/cy);
//		rounded_right_bottom_colorList[(xx+1)*4+3]=rgb_top[3]+rgb_delta[3]*(color_height/cy);
//		angle+=delta_angle;
//	}
//
//	//left bottom
//	rounded_left_bottom_vertList[0]=deltaHeight;
//	rounded_left_bottom_vertList[1]=cy-deltaHeight;
//	rounded_left_bottom_colorList[0]=rgb_top[0]+rgb_delta[0]*(rounded_left_bottom_vertList[1]/cy);
//	rounded_left_bottom_colorList[1]=rgb_top[1]+rgb_delta[1]*(rounded_left_bottom_vertList[1]/cy);
//	rounded_left_bottom_colorList[2]=rgb_top[2]+rgb_delta[2]*(rounded_left_bottom_vertList[1]/cy);
//	rounded_left_bottom_colorList[3]=rgb_top[3]+rgb_delta[3]*(rounded_left_bottom_vertList[1]/cy);
//
//	angle=90;
//	for(int xx=step;xx>=0;xx--)
//	{
//		rounded_left_bottom_vertList[(xx+1)*2+0]=rounded_left_bottom_vertList[0]+deltaHeight*gxMath::COSF(angle);
//		rounded_left_bottom_vertList[(xx+1)*2+1]=rounded_left_bottom_vertList[1]+deltaHeight*gxMath::SINF(angle);
//
//		float color_height=rounded_left_bottom_vertList[(xx+1)*2+1];
//		rounded_left_bottom_colorList[(xx+1)*4+0]=rgb_top[0]+rgb_delta[0]*(color_height/cy);
//		rounded_left_bottom_colorList[(xx+1)*4+1]=rgb_top[1]+rgb_delta[1]*(color_height/cy);
//		rounded_left_bottom_colorList[(xx+1)*4+2]=rgb_top[2]+rgb_delta[2]*(color_height/cy);
//		rounded_left_bottom_colorList[(xx+1)*4+3]=rgb_top[3]+rgb_delta[3]*(color_height/cy);
//		angle+=delta_angle;
//	}
//
//
//	glPushMatrix();
//	glTranslatef(x, y, 0);
//	glEnableClientState(GL_VERTEX_ARRAY);
//	glEnableClientState(GL_COLOR_ARRAY);
//	
//	//draw the horizontal rect
//
//	glVertexPointer(2, GL_FLOAT, 0, vertical_vertLst);
//	glColorPointer(4, GL_FLOAT, 0, vertical_colorLst);
//    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//
//	glVertexPointer(2, GL_FLOAT, 0, horizontal_vertLst);
//	glColorPointer(4, GL_FLOAT, 0, horizontal_colorLst);
//    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//
//
//	glVertexPointer(2, GL_FLOAT, 0, rounded_left_top_vertList);
//	glColorPointer(4, GL_FLOAT, 0, rounded_left_top_colorList);
//    glDrawArrays(GL_TRIANGLE_FAN, 0, (2+step));
//
//	glVertexPointer(2, GL_FLOAT, 0, rounded_right_top_vertList);
//	glColorPointer(4, GL_FLOAT, 0, rounded_right_top_colorList);
//    glDrawArrays(GL_TRIANGLE_FAN, 0, (2+step));
//
//	glVertexPointer(2, GL_FLOAT, 0, rounded_right_bottom_vertList);
//	glColorPointer(4, GL_FLOAT, 0, rounded_right_bottom_colorList);
//    glDrawArrays(GL_TRIANGLE_FAN, 0, (2+step));
//
//	glVertexPointer(2, GL_FLOAT, 0, rounded_left_bottom_vertList);
//	glColorPointer(4, GL_FLOAT, 0, rounded_left_bottom_colorList);
//    glDrawArrays(GL_TRIANGLE_FAN, 0, (2+step));
//
//	glDisableClientState(GL_COLOR_ARRAY);
//	glDisableClientState(GL_VERTEX_ARRAY);
//	glPopMatrix();
//}