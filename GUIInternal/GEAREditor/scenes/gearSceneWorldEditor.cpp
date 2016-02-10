#include "gearSceneWorldEditor.h"
#include "../EditorApp.h"
#include "../gui/geToolBarSeperator.h"
#include "../../../GEAREngine/src/hwShader/gxHWShader.h"
#ifdef _WIN32
#include "../../resource.h"
#endif
#if USE_NVPROFILER
#include "../../../GEAREngine/src/util/nvProfiler.h"
#include "../../../GEAREngine/src/util/nvProfiler.cpp"
#endif

gearSceneWorldEditor::gearSceneWorldEditor(geFontManager* fontmanager):
geWindow("World Editor", fontmanager)
{
	selectedObject=NULL;
	m_pMainWorldPtr=NULL;

	ambientLightHorizontalSlider=NULL;
	timeScaleHorizontalSlider=NULL;
	localOrGlobalAxisToolBarButton=NULL;
	gridViewToolBarButton=NULL;

	playGameToolBarButton = NULL;
	pauseGameToolBarButton = NULL;
	onlyLightPassToolBarButton = NULL;
	showOOBBToolBarButton = NULL;
	showOctreeToolBarButton = NULL;
	isMonoGameInitialized=false;

	translateGizmoToolBarButton = NULL;
	rotateGizmoToolBarButton = NULL;
	scaleGizmoToolBarButton = NULL;
	isEnablePostProcessorBlur=false;
	lastGLErrorID=0;
	stopFollowCam=true;
	noOfSelectedObjectTriangles=0;
}

gearSceneWorldEditor::~gearSceneWorldEditor()
{
#if USE_NVPROFILER
	GetNvPmApi()->Shutdown();
#endif
}

void gearSceneWorldEditor::onCreate()
{
#if USE_NVPROFILER
	nvProfiler::InitNVPM(EditorGEARApp::getMainRenderer()->getRenderingContext());
#endif

	monoWrapper::mono_engine_init(2);
	m_pMainWorldPtr = monoWrapper::mono_engine_getWorld(0);

	m_pMainWorldPtr->setWorldObserver(this);

	char metaDataFolder[512];
	memset(metaDataFolder, 0, sizeof(metaDataFolder));
	sprintf(metaDataFolder, "%s/%s", EditorGEARApp::getProjectHomeDirectory(), "MetaData");
	monoWrapper::mono_engine_setMetaFolder(monoWrapper::mono_engine_getWorld(0), metaDataFolder);
	monoWrapper::mono_engine_setMetaFolder(monoWrapper::mono_engine_getWorld(1), metaDataFolder);

	localOrGlobalAxisToolBarButton=new geToolBarButton(rendererGUI, "Local", getToolBar(), fontManagerGUI);
	localOrGlobalAxisToolBarButton->setGUIObserver(this);
	getToolBar()->appendToolBarControl(localOrGlobalAxisToolBarButton);
	isTransformThroughLocalAxis=true;

	translateGizmoToolBarButton=new geToolBarButton(rendererGUI, "t", getToolBar(), fontManagerGUI);
	translateGizmoToolBarButton->loadImage("res//icons16x16.png", 7, 153);
	translateGizmoToolBarButton->setGUIObserver(this);
	getToolBar()->appendToolBarControl(translateGizmoToolBarButton);
	rotateGizmoToolBarButton=new geToolBarButton(rendererGUI, "r", getToolBar(), fontManagerGUI);
	rotateGizmoToolBarButton->loadImage("res//icons16x16.png", 27, 153);
	rotateGizmoToolBarButton->setGUIObserver(this);
	getToolBar()->appendToolBarControl(rotateGizmoToolBarButton);
	scaleGizmoToolBarButton=new geToolBarButton(rendererGUI, "s", getToolBar(), fontManagerGUI);
	scaleGizmoToolBarButton->loadImage("res//icons16x16.png", 49, 153);
	scaleGizmoToolBarButton->setGUIObserver(this);
	getToolBar()->appendToolBarControl(scaleGizmoToolBarButton);

	//enable translate gizmo
	translateGizmoToolBarButton->buttonPressed(false);

	ambientLightHorizontalSlider = new geHorizontalSlider(fontManagerGUI);
	ambientLightHorizontalSlider->create(rendererGUI, getToolBar(), "slider", 0, GE_TOOLBAR_HEIGHT*0.35f, 70);
	ambientLightHorizontalSlider->setGUIObserver(this);
	ambientLightHorizontalSlider->setSliderValue(0.4f);
	getToolBar()->appendToolBarControl(ambientLightHorizontalSlider);

	geToolBarSeperator* seperator = new geToolBarSeperator(rendererGUI, getToolBar(), 20, fontManagerGUI);
	getToolBar()->appendToolBarControl(seperator);
	geToolBarSeperator* seperator2 = new geToolBarSeperator(rendererGUI, getToolBar(), 40, fontManagerGUI);
	getToolBar()->appendToolBarControl(seperator2);

	gridViewToolBarButton=new geToolBarButton(rendererGUI, "grid", getToolBar(), fontManagerGUI);
	gridViewToolBarButton->loadImage("res//icons16x16.png", 112, 384);
	//gridViewToolBarButton->setClientAreaSecondryActiveForeColor(0.5f, 0.3f, 0.2f, 1.0f);
	//gridViewToolBarButton->applyPrimaryColorToVBClientArea(EGRADIENT_VERTICAL_UP, 0.3f);
	gridViewToolBarButton->buttonPressed(false);
	getToolBar()->appendToolBarControl(gridViewToolBarButton);

	geToolBarSeperator* seperator3 = new geToolBarSeperator(rendererGUI, getToolBar(), 40, fontManagerGUI);
	getToolBar()->appendToolBarControl(seperator3);

	//play-pause-stop
	playGameToolBarButton=new geToolBarButton(rendererGUI, "play", getToolBar(), fontManagerGUI);
	playGameToolBarButton->loadImage("res//icons16x16.png", 26, 216);
	playGameToolBarButton->setGUIObserver(this);
	getToolBar()->appendToolBarControl(playGameToolBarButton);
	pauseGameToolBarButton=new geToolBarButton(rendererGUI, "pause", getToolBar(), fontManagerGUI);
	pauseGameToolBarButton->loadImage("res//icons16x16.png", 90, 216);
	pauseGameToolBarButton->setGUIObserver(this);
	getToolBar()->appendToolBarControl(pauseGameToolBarButton);
	//

	geToolBarSeperator* seperator4 = new geToolBarSeperator(rendererGUI, getToolBar(), 40, fontManagerGUI);
	getToolBar()->appendToolBarControl(seperator4);

	timeScaleHorizontalSlider = new geHorizontalSlider(fontManagerGUI);
	timeScaleHorizontalSlider->create(rendererGUI, getToolBar(), "slider", 0, GE_TOOLBAR_HEIGHT*0.35f, 130);
	timeScaleHorizontalSlider->setGUIObserver(this);
	timeScaleHorizontalSlider->setSliderValue(1.0f);
	getToolBar()->appendToolBarControl(timeScaleHorizontalSlider);

	geToolBarSeperator* seperator5 = new geToolBarSeperator(rendererGUI, getToolBar(), 40, fontManagerGUI);
	getToolBar()->appendToolBarControl(seperator5);

	onlyLightPassToolBarButton=new geToolBarButton(rendererGUI, "onlylightpass", getToolBar(), fontManagerGUI);
	onlyLightPassToolBarButton->loadImage("res//icons16x16.png", 153, 342);
	onlyLightPassToolBarButton->setGUIObserver(this);
	getToolBar()->appendToolBarControl(onlyLightPassToolBarButton);

	showOOBBToolBarButton=new geToolBarButton(rendererGUI, "showoobb", getToolBar(), fontManagerGUI);
	showOOBBToolBarButton->loadImage("res//icons16x16.png", 133, 258);
	showOOBBToolBarButton->setGUIObserver(this);
	getToolBar()->appendToolBarControl(showOOBBToolBarButton);

	showOctreeToolBarButton=new geToolBarButton(rendererGUI, "showoctree", getToolBar(), fontManagerGUI);
	showOctreeToolBarButton->loadImage("res//icons16x16.png", 70, 69);
	showOctreeToolBarButton->setGUIObserver(this);
	getToolBar()->appendToolBarControl(showOctreeToolBarButton);

	//create grid
	float startX=-500.0f;
	float startY=-500.0f;

	float oneGridSz=GX_ABS(2.0f*startX)/10.0f;
	float oneSubGridSz=oneGridSz/10.0f;
	float halfoneSubGridSz=0;//oneSubGridSz*0.5f;

	gridOuterBoxArray[0].set(startX, startY);
	gridOuterBoxArray[1].set(-startX, startY);
	gridOuterBoxArray[2].set(-startX, -startY);
	gridOuterBoxArray[3].set(startX, -startY);

	for(int x=0,cntrY=0, cntrX=0;x<10;x++)
	{
		if(x<9)
		{
			thickGridOnYAxisArray[x*2+0].x=startX+halfoneSubGridSz+(x+1)*oneGridSz;
			thickGridOnYAxisArray[x*2+0].y=startY;

			thickGridOnYAxisArray[x*2+1].x=startX+halfoneSubGridSz+(x+1)*oneGridSz;
			thickGridOnYAxisArray[x*2+1].y=-startY;

			thickGridOnXAxisArray[x*2+0].x=startX;
			thickGridOnXAxisArray[x*2+0].y=startY+halfoneSubGridSz+(x+1)*oneGridSz;

			thickGridOnXAxisArray[x*2+1].x=-startX;
			thickGridOnXAxisArray[x*2+1].y=startY+halfoneSubGridSz+(x+1)*oneGridSz;
		}

		for(int y=0;y<9;y++)
		{
			gridOnYAxisArray[cntrY].x=startX+oneSubGridSz+x*oneGridSz+y*oneSubGridSz;
			gridOnYAxisArray[cntrY++].y=startY;

			gridOnYAxisArray[cntrY].x=startX+oneSubGridSz+x*oneGridSz+y*oneSubGridSz;
			gridOnYAxisArray[cntrY++].y=-startY;

			gridOnXAxisArray[cntrX].x=startX;
			gridOnXAxisArray[cntrX++].y=startY+oneSubGridSz+x*oneGridSz+y*oneSubGridSz;

			gridOnXAxisArray[cntrX].x=-startX;
			gridOnXAxisArray[cntrX++].y=startY+oneSubGridSz+x*oneGridSz+y*oneSubGridSz;
		}
	}
	//

	//hwshader manager
	hwShaderManager = engine_getHWShaderManager();
	hwShaderManager->Init();

#if defined USE_FBO
	//fbo
	multiPassFBO.ReInitFBO(512, 512);
    multiPassFBO.CreateTextureBuffer();
    multiPassFBO.AttachTextureBuffer(0);
    multiPassFBO.CreateDepthBuffer();
    multiPassFBO.AttachDepthBuffer();
    multiPassFBO.UnBindFBO();

	brightPassFilter.init(&multiPassFBO);
	blurFilter.init(&brightPassFilter.getOutPutFBO());
	toneMappingFilter.init(&multiPassFBO, &blurFilter.getOutPutFBO());

#ifdef ENABLE_FOG
	fogFBO.ReInitFBO(512, 512);
    fogFBO.CreateTextureBuffer();
    fogFBO.AttachTextureBuffer(0);
    fogFBO.CreateDepthBuffer();
    fogFBO.AttachDepthBuffer();
    fogFBO.UnBindFBO();
#endif
#endif

	lightBillBoardSprite.setOffset(0, 0);
	lightBillBoardSprite.loadTexture(&geGUIManager::g_cTextureManager, "res//icons16x16.png");

	//soundEngine.init();
	//soundEngine.createListener();
	//SoundSource* snd=soundEngine.load("res//sounds//ingame_level5_44_16_s.sx");
	//snd->play();
}

void gearSceneWorldEditor::draw()
{
#ifdef LOG_GLERROR
	int err=glGetError();
	if(err!=GL_NO_ERROR)
	{
		printf("glGetError 0x%x\n", err);
	}
#endif

	//engine updte func
	CHECK_GL_ERROR(onDraw());

	//engine render funcs
	CHECK_GL_ERROR(drawShadowMapPass());
	CHECK_GL_ERROR(drawWorld());
	CHECK_GL_ERROR(drawFBO2FrameBuffer());

#if USE_BULLET
	glViewport(m_cPos.x+getIamOnLayout()->getPos().x, (rendererGUI->getViewPortSz().y)-(m_cPos.y+getIamOnLayout()->getPos().y+m_cSize.y), m_cSize.x, m_cSize.y-getTopMarginOffsetHeight());	
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
        0,      0,
        0,      0+cy,
        0+cx,   0+cy,
        0+cx,   0
    };
    
	float cszTileTexBuffer[]={
        0,1,
		0,0,
		1,0,
		1,1,
    };
  
	if(isEnablePostProcessorBlur)
	{
		//glDisable(GL_CULL_FACE);
		gxHWShader* shader=engine_getHWShaderManager()->GetHWShader(HW_BUILTIN_DEFAULT_SHADER_ONLY_BLUR_SHADER);
    
		shader->enableProgram();
   
		CHECK_GL_ERROR(glVertexAttribPointer(shader->getAttribLoc("a_vertex_coord_v4"), 2, GL_FLOAT, GL_FALSE, 0, cszTileBuffer));
		CHECK_GL_ERROR(glEnableVertexAttribArray(shader->getAttribLoc("a_vertex_coord_v4")));

		////////
		CHECK_GL_ERROR(glActiveTexture(GL_TEXTURE0));
		CHECK_GL_ERROR(glVertexAttribPointer(shader->getAttribLoc("a_uv_coord0_v2"), 2, GL_FLOAT, GL_FALSE, 0, cszTileTexBuffer));
		CHECK_GL_ERROR(glEnableVertexAttribArray(shader->getAttribLoc("a_uv_coord0_v2")));
		//arun:texissue glEnable(GL_TEXTURE_2D);
		CHECK_GL_ERROR(glBindTexture(GL_TEXTURE_2D, t));	

		shader->sendUniform1i("u_diffuse_texture", 0);

		matrix4x4f offset;
		offset.setPosition(-(-x+cx)*0.5f, -(-y+cy+getTopMarginOffsetHeight())*0.5f, -1.0f);
		const float* u_mvp_m4x4 = (*m_pMainWorldPtr->getRenderer()->getOrthoProjectionMatrix() * offset).getMatrix();
		shader->sendUniformTMfv("u_mvp_m4x4", u_mvp_m4x4, false, 4);
    
		CHECK_GL_ERROR(glDrawArrays(GL_TRIANGLE_FAN, 0, 4));
    
		//Disable all texture ops
		CHECK_GL_ERROR(glDisableVertexAttribArray(shader->getAttribLoc("a_uv_coord0_v2")));
		//arun:texissue glDisable(GL_TEXTURE_2D);
		CHECK_GL_ERROR(glBindTexture(GL_TEXTURE_2D, 0));
    
		CHECK_GL_ERROR(glDisableVertexAttribArray(shader->getAttribLoc("a_vertex_coord_v4")));
		CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER,0));
    
		shader->disableProgram();    
	}
	else
	{
		//glDisable(GL_CULL_FACE);
		glColor3f(1.0f, 1.0f, 1.0f);
 		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_FLOAT, 0, cszTileBuffer);
    
		glActiveTexture(GL_TEXTURE0);
		glClientActiveTexture(GL_TEXTURE0);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, 0, cszTileTexBuffer);
    
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, t);
		glPushMatrix();
		glTranslatef(-(-x+cx)*0.5f, -(-y+cy+getTopMarginOffsetHeight())*0.5f, -1.0f);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		glPopMatrix();

		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
	}
}

void gearSceneWorldEditor::drawFOGFBO(GLuint base_t, GLuint depth_t, float x, float y, float cx, float cy)
{
    float cszTileBuffer[]={
        0,      0,
        0,      0+cy,
        0+cx,   0+cy,
        0+cx,   0
    };
    
	float cszTileTexBuffer[]={
        0,1,
		0,0,
		1,0,
		1,1,
    };
  
	//glDisable(GL_CULL_FACE);
	gxHWShader* shader=NULL;
	stFog* fog_struct = monoWrapper::mono_engine_getWorld(0)->getRenderer()->getFog();

	switch(fog_struct->fog_type)
	{
	case stFog::FOG_LINEAR:
		{
			shader=engine_getHWShaderManager()->GetHWShader(HW_BUILTIN_DEFAULT_SHADER_ONLY_FOG_LINEAR_SHADER);
			shader->enableProgram();
			shader->sendUniform1f("fog_end", fog_struct->fog_end);
			shader->sendUniform1f("fog_scale", fog_struct->fog_scale);
		}
		break;
	case stFog::FOG_EXP:
		{
			shader=engine_getHWShaderManager()->GetHWShader(HW_BUILTIN_DEFAULT_SHADER_ONLY_FOG_EXP_SHADER);
			shader->enableProgram();
			shader->sendUniform1f("fog_density", fog_struct->fog_density);
		}
		break;
	case stFog::FOG_EXP2:
		{
			shader=engine_getHWShaderManager()->GetHWShader(HW_BUILTIN_DEFAULT_SHADER_ONLY_FOG_EXP2_SHADER);
			shader->enableProgram();
			shader->sendUniform1f("fog_density", fog_struct->fog_density);
		}
		break;
	}
	
	shader->sendUniform4fv("fog_color", &fog_struct->fog_color.x);

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	CHECK_GL_ERROR(glVertexAttribPointer(shader->getAttribLoc("a_vertex_coord_v4"), 2, GL_FLOAT, GL_FALSE, 0, cszTileBuffer));
	CHECK_GL_ERROR(glEnableVertexAttribArray(shader->getAttribLoc("a_vertex_coord_v4")));

	////////
	CHECK_GL_ERROR(glActiveTexture(GL_TEXTURE0));
	CHECK_GL_ERROR(glVertexAttribPointer(shader->getAttribLoc("a_uv_coord0_v2"), 2, GL_FLOAT, GL_FALSE, 0, cszTileTexBuffer));
	CHECK_GL_ERROR(glEnableVertexAttribArray(shader->getAttribLoc("a_uv_coord0_v2")));
	//arun:texissue glEnable(GL_TEXTURE_2D);
	CHECK_GL_ERROR(glBindTexture(GL_TEXTURE_2D, base_t));	
	shader->sendUniform1i("u_base_texture", 0);

	CHECK_GL_ERROR(glActiveTexture(GL_TEXTURE0+1));
	CHECK_GL_ERROR(glVertexAttribPointer(shader->getAttribLoc("a_uv_coord0_v2"), 2, GL_FLOAT, GL_FALSE, 0, cszTileTexBuffer));
	CHECK_GL_ERROR(glEnableVertexAttribArray(shader->getAttribLoc("a_uv_coord0_v2")));
	//arun:texissue glEnable(GL_TEXTURE_2D);
	CHECK_GL_ERROR(glBindTexture(GL_TEXTURE_2D, depth_t));	
	shader->sendUniform1i("u_depth_texture", 1);


	matrix4x4f offset;
	offset.setPosition(-(-x+cx)*0.5f, -(-y+cy+getTopMarginOffsetHeight())*0.5f, -1.0f);
	const float* u_mvp_m4x4 = (*m_pMainWorldPtr->getRenderer()->getOrthoProjectionMatrix() * offset).getMatrix();
	shader->sendUniformTMfv("u_mvp_m4x4", u_mvp_m4x4, false, 4);
    
	CHECK_GL_ERROR(glDrawArrays(GL_TRIANGLE_FAN, 0, 4));
    
	//Disable all texture ops
	CHECK_GL_ERROR(glDisableVertexAttribArray(shader->getAttribLoc("a_uv_coord0_v2")));
	//arun:texissue glDisable(GL_TEXTURE_2D);
	CHECK_GL_ERROR(glBindTexture(GL_TEXTURE_2D, 0));
    
	CHECK_GL_ERROR(glDisableVertexAttribArray(shader->getAttribLoc("a_vertex_coord_v4")));
	CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER,0));
    
	//glDisable(GL_BLEND);

	shader->disableProgram();
}

void gearSceneWorldEditor::onDraw()
{
	followObject(Timer::getDtinSec(), selectedObject);

	monoWrapper::mono_engine_update(m_pMainWorldPtr, Timer::getDtinSec()*Timer::getTimeScale());
	if(playGameToolBarButton->isButtonPressed() && !pauseGameToolBarButton->isButtonPressed())
	{
		if(isMonoGameInitialized)
		{
			m_pMainWorldPtr->updateMono();
			monoWrapper::mono_game_run(Timer::getDtinSec()*Timer::getTimeScale());
		}
	}
}

void gearSceneWorldEditor::drawShadowMapPass()
{
	m_pMainWorldPtr->renderShadow(object3d::eObject3dBase_RenderFlag_NormalRenderPass);
}

void gearSceneWorldEditor::drawWorld()
{
	multiPassFBO.BindFBO();
	//gxCamera* active_cam=m_pMainWorldPtr->getActiveCamera()->getCameraStructure();
	monoWrapper::mono_engine_resize(m_pMainWorldPtr, 
		m_cPos.x+getIamOnLayout()->getPos().x,
		(rendererGUI->getViewPortSz().y)-(m_cPos.y+getIamOnLayout()->getPos().y+m_cSize.y),
		m_cSize.x,
		m_cSize.y/*-getTopMarginOffsetHeight()*/,
		10.0f, 100000.0f);
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
	monoWrapper::mono_engine_render(m_pMainWorldPtr, NULL, object3d::eObject3dBase_RenderFlag_NormalRenderPass);
#endif

	CHECK_GL_ERROR(drawGrid());
	CHECK_GL_ERROR(drawSelectedObject());
	CHECK_GL_ERROR(drawOctree());
	multiPassFBO.UnBindFBO();

#ifdef ENABLE_FOG
	//postprocess fog
	glDisable(GL_DEPTH_TEST);
	fogFBO.BindFBO();
	CHECK_GL_ERROR(glViewport(0, 0, fogFBO.getFBOWidth(), fogFBO.getFBOHeight()));	
	CHECK_GL_ERROR(glClear(GL_COLOR_BUFFER_BIT/* | GL_DEPTH_BUFFER_BIT*/));
	drawFOGFBO(multiPassFBO.getFBOTextureBuffer(0), multiPassFBO.getFBODepthBuffer(), 0.0f, 0.0f, fogFBO.getFBOWidth(), fogFBO.getFBOHeight());
	fogFBO.UnBindFBO();
	//
#endif
}

void gearSceneWorldEditor::drawGrid()
{
	//grid
	if(gridViewToolBarButton->isButtonPressed())
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

		glVertexAttribPointer(shader->getAttribLoc("a_vertex_coord_v4"), 2, GL_FLOAT, GL_FALSE, 0, &gridOnYAxisArray[0].x);
		glDrawArrays(GL_LINES, 0, 90*2);
		glVertexAttribPointer(shader->getAttribLoc("a_vertex_coord_v4"), 2, GL_FLOAT, GL_FALSE, 0, &gridOnXAxisArray[0].x);
		glDrawArrays(GL_LINES, 0, 90*2);

		diffuseClr.set(0.1f, 0.1f, 0.1f, 1.0f);
		shader->sendUniform4fv("u_diffuse_v4", &diffuseClr.x);

		glVertexAttribPointer(shader->getAttribLoc("a_vertex_coord_v4"), 2, GL_FLOAT, GL_FALSE, 0, &thickGridOnYAxisArray[0].x);
		glDrawArrays(GL_LINES, 0, 9*2);
		glVertexAttribPointer(shader->getAttribLoc("a_vertex_coord_v4"), 2, GL_FLOAT, GL_FALSE, 0, &thickGridOnXAxisArray[0].x);
		glDrawArrays(GL_LINES, 0, 9*2);

		glVertexAttribPointer(shader->getAttribLoc("a_vertex_coord_v4"), 2, GL_FLOAT, GL_FALSE, 0, &gridOuterBoxArray[0].x);
		glDrawArrays(GL_LINE_LOOP, 0, 4);

		glDisableVertexAttribArray(shader->getAttribLoc("a_vertex_coord_v4"));
		shader->disableProgram();
	}
	//
}

void gearSceneWorldEditor::drawCameraFrustum(Camera* camera, gxHWShader* shader)
{
	gxFrustumf& frustum=camera->getFrustum();

	float lineAry[]={
		frustum.frustumVertices[0].x, frustum.frustumVertices[0].y, frustum.frustumVertices[0].z,		//0	near
		frustum.frustumVertices[1].x, frustum.frustumVertices[1].y, frustum.frustumVertices[1].z,		//1
		frustum.frustumVertices[2].x, frustum.frustumVertices[2].y, frustum.frustumVertices[2].z,		//2
		frustum.frustumVertices[3].x, frustum.frustumVertices[3].y, frustum.frustumVertices[3].z,		//3

		frustum.frustumVertices[4].x, frustum.frustumVertices[4].y, frustum.frustumVertices[4].z,		//4	far
		frustum.frustumVertices[5].x, frustum.frustumVertices[5].y, frustum.frustumVertices[5].z,		//5
		frustum.frustumVertices[6].x, frustum.frustumVertices[6].y, frustum.frustumVertices[6].z,		//6
		frustum.frustumVertices[7].x, frustum.frustumVertices[7].y, frustum.frustumVertices[7].z,		//7

		frustum.frustumVertices[0].x, frustum.frustumVertices[0].y, frustum.frustumVertices[0].z,		//0	near
		frustum.frustumVertices[4].x, frustum.frustumVertices[4].y, frustum.frustumVertices[4].z,		//4	far

		frustum.frustumVertices[1].x, frustum.frustumVertices[1].y, frustum.frustumVertices[1].z,		//1
		frustum.frustumVertices[5].x, frustum.frustumVertices[5].y, frustum.frustumVertices[5].z,		//5

		frustum.frustumVertices[2].x, frustum.frustumVertices[2].y, frustum.frustumVertices[2].z,		//2
		frustum.frustumVertices[6].x, frustum.frustumVertices[6].y, frustum.frustumVertices[6].z,		//6

		frustum.frustumVertices[3].x, frustum.frustumVertices[3].y, frustum.frustumVertices[3].z,		//3
		frustum.frustumVertices[7].x, frustum.frustumVertices[7].y, frustum.frustumVertices[7].z,		//7
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

			lightBillBoardSprite.setClip(277, 383, 18, 18);
            vector3f yaxis(m_pMainWorldPtr->getActiveCamera()->getYAxis());
            vector3f zaxis(m_pMainWorldPtr->getActiveCamera()->getZAxis());
            
			lightBillBoardSprite.setDirection(&yaxis, &zaxis);
			lightBillBoardSprite.scale(distance_frm_cam*0.5f);
			const float* u_mvp_m4x4= (*m_pMainWorldPtr->getRenderer()->getViewProjectionMatrix() * (*light->getWorldMatrix() * lightBillBoardSprite)).getMatrix();
			shader->sendUniformTMfv("u_mvp_m4x4", u_mvp_m4x4, false, 4);

			glDisable(GL_CULL_FACE);
			lightBillBoardSprite.draw(shader);
			glEnable(GL_CULL_FACE);
		}
		shader->disableProgram();
	}
	//

	if(selectedObject && m_pMainWorldPtr->getActiveCamera())
	{
		gxHWShader* shader = engine_getHWShaderManager()->GetHWShader(HW_BUILTIN_DEFAULT_SHADER_ONLY_DIFFUSE_WITH_COLOR_PTR);
		shader->enableProgram();

		float distance_frm_cam=(selectedObject->getWorldMatrix()->getPosition()-m_pMainWorldPtr->getActiveCamera()->getWorldMatrix()->getPosition()).length();
		if(distance_frm_cam<12.0f)
			distance_frm_cam=1.0f;
		else
			distance_frm_cam/=12.0f;

		const float* u_mvp_m4x4_local=NULL;
		const float* u_mvp_m4x4_world=NULL;
		matrix4x4f noscale_world_matrix(*selectedObject->getWorldMatrix());
		noscale_world_matrix.noScale();
		u_mvp_m4x4_local = (*m_pMainWorldPtr->getRenderer()->getViewProjectionMatrix() * noscale_world_matrix).getMatrix();

		matrix4x4f globalAxisTM;
		globalAxisTM.setPosition(selectedObject->getWorldMatrix()->getPosition());
		u_mvp_m4x4_world = (*m_pMainWorldPtr->getRenderer()->getViewProjectionMatrix() * globalAxisTM).getMatrix();

		//glEnable(GL_COLOR_MATERIAL);
		if(isTransformThroughLocalAxis)
			shader->sendUniformTMfv("u_mvp_m4x4", u_mvp_m4x4_local, false, 4);
		else
			shader->sendUniformTMfv("u_mvp_m4x4", u_mvp_m4x4_world, false, 4);
		geUtil::drawGizmo(distance_frm_cam, shader, selectedAxisID);

		shader->disableProgram();

		shader = engine_getHWShaderManager()->GetHWShader(HW_BUILTIN_DEFAULT_SHADER_ONLY_DIFFUSE);
		shader->enableProgram();

		shader->sendUniformTMfv("u_mvp_m4x4", u_mvp_m4x4_local, false, 4);
#if USE_BULLET
		if(selectedObject->getRigidBody())
			shader->sendUniform4f("u_diffuse_v4", 0.25f, 0.0f, 0.62f, 1.0f);
		else
#endif
			shader->sendUniform4f("u_diffuse_v4", 0.25f, 0.4f, 0.62f, 1.0f);
		if(showOOBBToolBarButton->isButtonPressed())
		{
			selectedObject->getOOBB().draw(shader);
		}
		else
		{
			shader->sendUniformTMfv("u_mvp_m4x4", m_pMainWorldPtr->getRenderer()->getViewProjectionMatrix()->getMatrix(), false, 4);
			shader->sendUniform4f("u_diffuse_v4", 0.6f, 0.4f, 0.62f, 1.0f);
			selectedObject->getAABB().draw(shader);
		}

		if(selectedObject->isBaseFlag(object3d::eObject3dBaseFlag_Visible) && selectedObject->getID()==OBJECT3D_CAMERA)
		{
			shader->sendUniformTMfv("u_mvp_m4x4", u_mvp_m4x4_local, false, 4);
			shader->sendUniform4f("u_diffuse_v4", 0.6f, 0.6f, 0.6f, 1.0f);
			((Camera*)selectedObject)->drawFrustum(shader);
		}

		//glDisable(GL_COLOR_MATERIAL);
		shader->disableProgram();
	}
}

void gearSceneWorldEditor::drawOctree()
{
	if(showOctreeToolBarButton->isButtonPressed() && m_pMainWorldPtr->getOctree())
	{
		gxHWShader* shader = engine_getHWShaderManager()->GetHWShader(HW_BUILTIN_DEFAULT_SHADER_ONLY_DIFFUSE);
		shader->enableProgram();

		shader->sendUniformTMfv("u_mvp_m4x4", m_pMainWorldPtr->getRenderer()->getViewProjectionMatrix()->getMatrix(), false, 4);
		m_pMainWorldPtr->getOctree()->drawOctree(m_pMainWorldPtr->getOctree()->getRoot(), shader);
		
		shader->disableProgram();
	}
}

void gearSceneWorldEditor::drawCompas()
{
	glEnable(GL_DEPTH_TEST);

	//world compas 
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
}

void gearSceneWorldEditor::drawStats()
{
	glPushMatrix();
	glTranslatef(-m_cSize.x*0.5f, -(m_cSize.y+getTopMarginOffsetHeight())*0.5f+getTopMarginOffsetHeight(), -1.0f);
		char buffer[128];
		sprintf(buffer, "FPS : %3.2f", Timer::getFPS());
		geFontManager::g_pFontArial10_84Ptr->drawString(buffer, 0, geFontManager::g_pFontArial10_84Ptr->getLineHeight(), m_cSize.x);
		int cnt=3;
		sprintf(buffer, "OpenGL Renderer : %s", glGetString(GL_RENDERER));
		geFontManager::g_pFontArial10_84Ptr->drawString(buffer, 0, geFontManager::g_pFontArial10_84Ptr->getLineHeight()*cnt++, m_cSize.x);
        sprintf(buffer, "OpenGL Version : %s", glGetString(GL_VERSION));
        geFontManager::g_pFontArial10_84Ptr->drawString(buffer, 0, geFontManager::g_pFontArial10_84Ptr->getLineHeight()*cnt++, m_cSize.x);
		sprintf(buffer, "GLSL %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
		geFontManager::g_pFontArial10_84Ptr->drawString(buffer, 0, geFontManager::g_pFontArial10_84Ptr->getLineHeight()*cnt++, m_cSize.x);
		sprintf(buffer, "TimeScale : %1.2f", timeScaleHorizontalSlider->getSliderValue());
		geFontManager::g_pFontArial10_84Ptr->drawString(buffer, 0, geFontManager::g_pFontArial10_84Ptr->getLineHeight()*cnt++, m_cSize.x);
		sprintf(buffer, "nTris : %d", m_pMainWorldPtr->getRenderer()->noOfTrianglesRendered);
		geFontManager::g_pFontArial10_84Ptr->drawString(buffer, 0, geFontManager::g_pFontArial10_84Ptr->getLineHeight()*cnt++, m_cSize.x);
		sprintf(buffer, "nDrawCalls : %d", m_pMainWorldPtr->getRenderer()->noOfDrawCalls);
		geFontManager::g_pFontArial10_84Ptr->drawString(buffer, 0, geFontManager::g_pFontArial10_84Ptr->getLineHeight()*cnt++, m_cSize.x);
		sprintf(buffer, "nSelectedObjectTriangles : %d", noOfSelectedObjectTriangles);
		geFontManager::g_pFontArial10_84Ptr->drawString(buffer, 0, geFontManager::g_pFontArial10_84Ptr->getLineHeight()*cnt++, m_cSize.x);

		sprintf(buffer, "Total Material : %d", (int)monoWrapper::mono_engine_getWorld(0)->getMaterialList()->size());
		geFontManager::g_pFontArial10_84Ptr->drawString(buffer, 0, geFontManager::g_pFontArial10_84Ptr->getLineHeight()*cnt++, m_cSize.x);
		sprintf(buffer, "Total Animation : %d", (int)monoWrapper::mono_engine_getWorld(0)->getAnimationSetList()->size());
		geFontManager::g_pFontArial10_84Ptr->drawString(buffer, 0, geFontManager::g_pFontArial10_84Ptr->getLineHeight()*cnt++, m_cSize.x);
		////int last_gl_err=glGetError();
		////if(last_gl_err!=GL_NO_ERROR)
		////	lastGLErrorID=last_gl_err;
		//sprintf(buffer, "glGetError : 0x%x", lastGLErrorID);
		//geFontManager::g_pFontArial10_84Ptr->drawString(buffer, 0, geFontManager::g_pFontArial10_84Ptr->getLineHeight()*cnt++, m_cSize.x);

//        sprintf(buffer, "mousePrevPosition : %f, %f", mousePrevPosition.x, mousePrevPosition.y);
//        geFontManager::g_pFontArial10_84Ptr->drawString(buffer, 0, geFontManager::g_pFontArial10_84Ptr->getLineHeight()*cnt++, m_cSize.x);

		sprintf(buffer, "Total Layer Objects : %d", m_pMainWorldPtr->getLayerManager()->getTotalLayerObject());
		geFontManager::g_pFontArial10_84Ptr->drawString(buffer, 0, geFontManager::g_pFontArial10_84Ptr->getLineHeight()*cnt++, m_cSize.x);

		geGUIBase* selectedNodeInHirarchy=EditorGEARApp::getSceneHierarchy()->getSelectedTreeNode();
		if(selectedNodeInHirarchy)
		{
			object3d* obj=(object3d*)selectedNodeInHirarchy->getUserData();
			if(obj && obj->getAnimationController())
			{
				sprintf(buffer, "Current Frame : %4.2f", obj->getAnimationController()->getCurrentFrame());
				geFontManager::g_pFontArial10_84Ptr->drawString(buffer, 0, 0+geFontManager::g_pFontArial10_84Ptr->getLineHeight()*cnt++, m_cSize.x);
			}
		}

#if USE_NVPROFILER
		nvProfiler::SampleAndRenderStats();
#endif

		drawCompas();
	glPopMatrix();
	//
}

void gearSceneWorldEditor::drawFBO2FrameBuffer()
{
	glViewport(m_cPos.x + getIamOnLayout()->getPos().x,
		(rendererGUI->getViewPortSz().y) - (m_cPos.y + getIamOnLayout()->getPos().y + m_cSize.y) - getTopMarginOffsetHeight(),
		m_cSize.x,
		m_cSize.y /*- getTopMarginOffsetHeight()*/);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glLoadMatrixf(m_pMainWorldPtr->getRenderer()->getOrthoProjectionMatrix()->getMatrix());
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);
#if defined USE_FBO

	int thumbnailFBOSz = 200;
	//bright pass
	brightPassFilter.beginBlit();
	brightPassFilter.blit(monoWrapper::mono_engine_getWorld(0)->getRenderer());
	brightPassFilter.endBlit();
	//
	//blur pass
	blurFilter.beginBlit();
	blurFilter.blit(monoWrapper::mono_engine_getWorld(0)->getRenderer());
	blurFilter.endBlit();
	//

	//tonemapping pass
	toneMappingFilter.beginBlit();
	toneMappingFilter.blit(monoWrapper::mono_engine_getWorld(0)->getRenderer());
	toneMappingFilter.endBlit();
	//

	drawFBO(multiPassFBO.getFBOTextureBuffer(0),
		0.0f,
		0.0f + getTopMarginOffsetHeight(),
		m_cSize.x,
		m_cSize.y);

	drawFBO(multiPassFBO.getFBOTextureBuffer(0), m_cSize.x - (thumbnailFBOSz + 10), -getTopMarginOffsetHeight() + m_cSize.y - (thumbnailFBOSz + 10), thumbnailFBOSz, thumbnailFBOSz);
	drawFBO(brightPassFilter.getOutPutFBO().getFBOTextureBuffer(0), m_cSize.x - (thumbnailFBOSz + 10), -getTopMarginOffsetHeight() + m_cSize.y - (thumbnailFBOSz + 10) * 3, thumbnailFBOSz, thumbnailFBOSz);
	drawFBO(blurFilter.getOutPutFBO().getFBOTextureBuffer(0), m_cSize.x - (thumbnailFBOSz + 10), -getTopMarginOffsetHeight() + m_cSize.y - (thumbnailFBOSz + 10) * 5, thumbnailFBOSz, thumbnailFBOSz);

	//shadow maps
	int nLight = -1;
	int offset = 10;
	for (std::vector<gxLight*>::iterator it = m_pMainWorldPtr->getLightList()->begin(); it != m_pMainWorldPtr->getLightList()->end(); ++it)
	{
		nLight++;
		gxLight* light = *it;
		if (!light->isBaseFlag(object3d::eObject3dBaseFlag_Visible))
			continue;

		drawFBO(light->getShadowMapFBO().getFBODepthBuffer(), offset + nLight * (thumbnailFBOSz + 10) + (nLight + 1) * (thumbnailFBOSz + 10) - m_cSize.x, -getTopMarginOffsetHeight() + m_cSize.y - (thumbnailFBOSz + 10), thumbnailFBOSz, thumbnailFBOSz);
	}
	//

#ifdef ENABLE_FOG
		drawFBO(fogFBO.getFBOTextureBuffer(0), m_cSize.x-310, m_cSize.y-310, 300, 300);
		drawFBO(multiPassFBO.getFBODepthBuffer(), -310, m_cSize.y-310, 300, 300);
#endif
#endif

	//draw statistics
	drawStats();
}

void gearSceneWorldEditor::preWorldRender()
{

}

void gearSceneWorldEditor::postWorldRender()
{
	////glBegin(GL_LINES);
	////glVertex3fv(&minV.x);
	////glVertex3fv(&maxV.x);
	////glEnd();

	//glDisable(GL_DEPTH_TEST);
	//glPushMatrix();
	//glTranslatef(debugPosition.x, debugPosition.y, debugPosition.z);
	////glColor4f(1, 0 , 0, 1);
	//glutWireSphere(0.1f, 3, 3);
	//glPopMatrix();
	////glPushMatrix();
	//////glTranslatef(testSpehere.x, testSpehere.y, testSpehere.z);
	////glColor4f(1, 0 , 0, 1);
	////glutWireSphere(0.1f, 3, 3);
	////glPopMatrix();
	//glEnable(GL_DEPTH_TEST);
}

void gearSceneWorldEditor::startFollowCam()
{
	if(playGameToolBarButton->isButtonPressed())
	{
		if(!pauseGameToolBarButton->isButtonPressed())
			return;
	}
	if(selectedObject==NULL) return;
	if(selectedObject->getID()==OBJECT3D_CAMERA) return;

	stopFollowCam=false;
}

void gearSceneWorldEditor::followObject(float dt, object3d* chasedObj)
{
	if(chasedObj && chasedObj->getID()==OBJECT3D_CAMERA)
	{
		stopFollowCam=true;
		return;
	}
	if(dt>0.1f || stopFollowCam) return;
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
		stopFollowCam=true;
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
		multiPassFBO.ReInitFBO(cx, cy);
        multiPassFBO.CreateTextureBuffer();
        multiPassFBO.AttachTextureBuffer(0);
		multiPassFBO.CreateDepthBuffer();
		multiPassFBO.AttachDepthBuffer();
		multiPassFBO.UnBindFBO();

		brightPassFilter.resize(cx, cy);
		blurFilter.resize(cx, cy);
		toneMappingFilter.resize(cx, cy);

#ifdef ENABLE_FOG
		fogFBO.ReInitFBO(cx, cy);
        fogFBO.CreateTextureBuffer();
        fogFBO.AttachTextureBuffer(0);
		fogFBO.CreateDepthBuffer();
		fogFBO.AttachDepthBuffer();
		fogFBO.UnBindFBO();
#endif

#if USE_NVPROFILER
		gTraceDisplay_DisplayW=cx;
		gTraceDisplay_DisplayH=cy;
#endif
	}
#endif

	geWindow::onSize(cx, cy, flag);
}

bool gearSceneWorldEditor::onMouseLButtonDown(float x, float y, int nFlag)
{
	if(!isPointInsideClientArea(x, y))
	{
		selectedAxisID=-1;
		return true;
	}
	//monoWrapper::mono_engine_mouseLButtonDown(m_pMainWorldPtr, x, y, nFlag);
	if(!m_pMainWorldPtr || !m_pMainWorldPtr->getActiveCamera()) return true;

	vector3f minV;
	vector3f rayminV;
	vector3f maxV;
	vector3f i1, i2, testSpehere;

	bool bTranslateGizmo=translateGizmoToolBarButton->isButtonPressed();
	bool bRotateGizmo=rotateGizmoToolBarButton->isButtonPressed();
	bool bScaleGizmo=scaleGizmoToolBarButton->isButtonPressed();

	if(!bTranslateGizmo && !bRotateGizmo && !bScaleGizmo)
		return true;

	GLdouble viewTM[16];
	GLdouble projectionTM[16];
	for(int m=0;m<16;m++)
	{
		viewTM[m]=m_pMainWorldPtr->getActiveCamera()->getViewMatrix()->getMatrix()[m];
		projectionTM[m]=m_pMainWorldPtr->getActiveCamera()->getProjectionMatrix()->getMatrix()[m];
	}

	gxRectf viewPortRect=m_pMainWorldPtr->getRenderer()->getViewPortRect();
	GLdouble x1, y1, z1;
	GLdouble x2, y2, z2;
	GLdouble x3, y3, z3;
	GLfloat depth;

	glEnable(GL_DEPTH_TEST);
	multiPassFBO.BindFBO();
	CHECK_GL_ERROR(glReadPixels(x, viewPortRect.m_size.y - y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth));
	multiPassFBO.UnBindFBO();
	z1=z2=0.0f;

	GLint viewport[4]={(int)viewPortRect.m_pos.x, (int)viewPortRect.m_pos.y, (int)viewPortRect.m_size.x, (int)viewPortRect.m_size.y};
	//float ptY=Scene::getCommonData()->getScreenHeight();
	CHECK_GL_ERROR(gluUnProject(x, viewPortRect.m_size.y - y, 0, viewTM, projectionTM, viewport, &x1, &y1, &z1));
	CHECK_GL_ERROR(gluUnProject(x, viewPortRect.m_size.y - y, 1, viewTM, projectionTM, viewport, &x2, &y2, &z2));
	CHECK_GL_ERROR(gluUnProject(x, viewPortRect.m_size.y - y, depth, viewTM, projectionTM, viewport, &x3, &y3, &z3));
	
	minV.set(x1, y1, z1);
	maxV.set(x2, y2, z2);
	rayminV.set(x3, y3, z3);

	selectedAxisID=0;
	if(selectedObject)
	{
		matrix4x4f noscale_world_matrix(*selectedObject->getWorldMatrix());
		noscale_world_matrix.noScale();
		float distance_frm_cam=(selectedObject->getWorldMatrix()->getPosition()-m_pMainWorldPtr->getActiveCamera()->getWorldMatrix()->getPosition()).length();
		if(distance_frm_cam<12.0f)
			distance_frm_cam=1.0f;
		else
			distance_frm_cam/=12.0f;

		//x-gizmo
		vector3f gizmo_origin(selectedObject->getWorldMatrix()->getPosition());
		vector3f xgizmo_max(distance_frm_cam, 0, 0);
		vector3f ygizmo_max(0, distance_frm_cam, 0);
		vector3f zgizmo_max(0, 0, distance_frm_cam);

		if(isTransformThroughLocalAxis)
		{
			xgizmo_max=noscale_world_matrix * xgizmo_max ;
			ygizmo_max=noscale_world_matrix * ygizmo_max ;
			zgizmo_max=noscale_world_matrix * zgizmo_max ;
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
					selectedAxisID=1;
					float pu;
					vector3f plane_normal((isTransformThroughLocalAxis)?selectedObject->getWorldMatrix()->getZAxis():vector3f(0, 0, 1));
					if(gxUtil::lineSegmentAndPlaneIntersection(minV, maxV, plane_normal, selectedObject->getWorldMatrix()->getPosition(), pu))
					{
						mousePreviousPositionInWorld = minV+(maxV-minV)*pu;
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
					selectedAxisID=2;
					float pu;
					vector3f plane_normal((isTransformThroughLocalAxis)?selectedObject->getWorldMatrix()->getZAxis():vector3f(0, 0, 1));
					if(gxUtil::lineSegmentAndPlaneIntersection(minV, maxV, plane_normal, selectedObject->getWorldMatrix()->getPosition(), pu))
					{
						mousePreviousPositionInWorld = minV+(maxV-minV)*pu;
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
					selectedAxisID=3;
					float pu;
					vector3f plane_normal((isTransformThroughLocalAxis)?selectedObject->getWorldMatrix()->getYAxis():vector3f(0, 1, 0));
					if(gxUtil::lineSegmentAndPlaneIntersection(minV, maxV, plane_normal, selectedObject->getWorldMatrix()->getPosition(), pu))
					{
						mousePreviousPositionInWorld = minV+(maxV-minV)*pu;
					}
				}
			}
		}
	}

	debugPosition=minV;
	if(selectedAxisID==0 && m_pMainWorldPtr->getOctree())
	{
		//multiPassFBO.BindFBO();
		//glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_DOUBLE, &depth);
		//multiPassFBO.UnBindFBO();
		vector3f rayOrig(rayminV);
		vector3f rayDir(maxV-rayminV);
		rayDir.normalize();
		object3d* pickedObj = m_pMainWorldPtr->getOctree()->pickBruteForce(rayOrig, rayDir);

		if(pickedObj!=selectedObject && pickedObj!=NULL)
		{
			EditorGEARApp::getSceneHierarchy()->selectObject3dInTreeView(pickedObj);
		}
	}

	mousePrevPosition.set(x, y);
	return true;
}

bool gearSceneWorldEditor::onMouseLButtonUp(float x, float y, int nFlag)
{
	if(!isPointInsideClientArea(x, y))
	{
		return true;
	}

	if(selectedAxisID>0)
		EditorGEARApp::getSceneHierarchy()->recreateOctree();

	selectedAxisID=-1;
	monoWrapper::mono_engine_mouseLButtonUp(m_pMainWorldPtr, x, y, nFlag);
	mousePrevPosition.set(x, y);
	return true;
}

bool gearSceneWorldEditor::onMouseRButtonDown(float x, float y, int nFlag)
{
	if(!isPointInsideClientArea(x, y))
	{
		selectedAxisID=-1;
		return true;
	}
	monoWrapper::mono_engine_mouseRButtonDown(m_pMainWorldPtr, x, y, nFlag);
	mousePrevPosition.set(x, y);
	return true;
}

void gearSceneWorldEditor::onMouseRButtonUp(float x, float y, int nFlag)
{
	if(!isPointInsideClientArea(x, y))
	{
		selectedAxisID=-1;
		return;
	}
	monoWrapper::mono_engine_mouseRButtonUp(m_pMainWorldPtr, x, y, nFlag);
	mousePrevPosition.set(x, y);
}

bool gearSceneWorldEditor::onMouseMove(float x, float y, int flag)
{
	if(!isPointInsideClientArea(x, y)) return false;
	//monoWrapper::mono_engine_mouseMove(m_pMainWorldPtr, x, y, flag);
	Camera* camera=m_pMainWorldPtr->getActiveCamera();
	
	if(!camera) return false;

	float dx	= (x-mousePrevPosition.x);
	float dy	= (y-mousePrevPosition.y);

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
		if(selectedObject)
			aVect=selectedObject->getWorldMatrix()->getPosition();	//can modify this later to rotate around mesh center

		float factor=1.0f;
		if(flag&MK_SHIFT)
			factor=0.01f;
		camera->rotateArb(-0.5f*dx*factor, &aUP.x, aVect);
		vector3f left=camera->getXAxis();
		camera->rotateArb(-0.5f*dy*factor, &left.x, aVect);
	}
	else if(flag&MK_LBUTTON)
	{
		bool bTranslateGizmo=translateGizmoToolBarButton->isButtonPressed();
		bool bRotateGizmo=rotateGizmoToolBarButton->isButtonPressed();
		bool bScaleGizmo=scaleGizmoToolBarButton->isButtonPressed();

		if((bTranslateGizmo || bRotateGizmo || bScaleGizmo))
		{
			if(selectedObject && selectedAxisID>0)
			{
				vector3f minV;
				vector3f maxV;

				//float d=camera->getPosition().length();
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
				GLint viewport[4]={(int)viewPortRect.m_pos.x, (int)viewPortRect.m_pos.y, (int)viewPortRect.m_size.x, (int)viewPortRect.m_size.y};
				//float ptY=Scene::getCommonData()->getScreenHeight();
				CHECK_GL_ERROR(gluUnProject(x, viewPortRect.m_size.y - y, 0, viewTM, projectionTM, viewport, &x1, &y1, &z1));
				CHECK_GL_ERROR(gluUnProject(x, viewPortRect.m_size.y - y, 1, viewTM, projectionTM, viewport, &x2, &y2, &z2));

				minV.set(x1, y1, z1);
				maxV.set(x2, y2, z2);

				vector3f planeNormalArray[3]={

					(isTransformThroughLocalAxis)?selectedObject->getWorldMatrix()->getZAxis():vector3f(0, 0, 1),
					(isTransformThroughLocalAxis)?selectedObject->getWorldMatrix()->getZAxis():vector3f(0, 0, 1),
					(isTransformThroughLocalAxis)?selectedObject->getWorldMatrix()->getYAxis():vector3f(0, 1, 0)
				};

				float pu;
				if(gxUtil::lineSegmentAndPlaneIntersection(minV, maxV, planeNormalArray[selectedAxisID-1], selectedObject->getWorldMatrix()->getPosition(), pu))
				{
					vector3f current_pos_in_world(minV+(maxV-minV)*pu);
					vector3f diff_in_world(current_pos_in_world-mousePreviousPositionInWorld);
					vector3f transformed;
					transformed=(isTransformThroughLocalAxis)?(selectedObject->getWorldMatrix()->getInverse() % diff_in_world):selectedObject->getWorldMatrix()->getPosition() + diff_in_world;
					//transformed=selectedObject->getWorldMatrix()->getInverse() % diff_in_world;

					if(isTransformThroughLocalAxis)
					{
						if(bTranslateGizmo)
						{
							if(selectedAxisID==1)
								selectedObject->updateLocalPositionf(transformed.x, 0, 0);
							else if(selectedAxisID==2)
								selectedObject->updateLocalPositionf(0, transformed.y, 0);
							else if(selectedAxisID==3)
								selectedObject->updateLocalPositionf(0, 0, transformed.z);
						}
						else if(bRotateGizmo)
						{
							if(selectedAxisID==1)
								selectedObject->rotateLocalXf(0.5f*dy);
							else if(selectedAxisID==2)
								selectedObject->rotateLocalYf(-0.5f*dy);
							else if(selectedAxisID==3)
								selectedObject->rotateLocalZf(0.5f*dx);
						}
						else if(bScaleGizmo)
						{
							if(selectedAxisID==1)
								selectedObject->scaleX(transformed.x);
							else if(selectedAxisID==2)
								selectedObject->scaleY(transformed.y);
							else if(selectedAxisID==3)
								selectedObject->scaleZ(transformed.z);
						}
					}
					else
					{
						vector3f right(1, 0, 0);
						right=right*diff_in_world.x;
						if(bTranslateGizmo)
						{
							if(selectedAxisID==1)
								selectedObject->updatePositionf(right.x, 0, 0);
							else if(selectedAxisID==2)
								selectedObject->updatePositionf(0, diff_in_world.y, 0);
							else if(selectedAxisID==3)
								selectedObject->updatePositionf(0, 0, diff_in_world.z);
						}
					}

					if(bTranslateGizmo || bRotateGizmo || bScaleGizmo)
					{
						m_pMainWorldPtr->transformationChangedf();
						EditorGEARApp::getScenePropertyEditor()->updateTransformPropertyOfCurrentSelectedObject();
					}

					mousePreviousPositionInWorld = current_pos_in_world;
				}
			}
		}
	}

	//DEBUG_PRINT("%f, %f\n", delta.x, delta.y);
	mousePrevPosition.set(x, y);

	return true;
}

void gearSceneWorldEditor::onMouseWheel(int zDelta, int x, int y, int flag)
{
	//if(!isPointInsideClientArea(x, y)) return;

	stopFollowCam=true;
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
	if(localOrGlobalAxisToolBarButton==btn)
	{
		if(strcmp(localOrGlobalAxisToolBarButton->getName(), "Local")==0)
		{
			localOrGlobalAxisToolBarButton->setName("Global");
			isTransformThroughLocalAxis=false;
		}
		else
		{
			localOrGlobalAxisToolBarButton->setName("Local");
			isTransformThroughLocalAxis=true;
		}
		localOrGlobalAxisToolBarButton->buttonNormal(true);
	}
	else if(playGameToolBarButton==btn)
	{
		if(playGameToolBarButton->isButtonPressed())
		{
			EditorGEARApp::getSceneConsole()->appendConsoleRunRootNode();
			monoWrapper::mono_game_start();
			isMonoGameInitialized=true;
		}
		else
		{
			pauseGameToolBarButton->buttonNormal(true);
			isMonoGameInitialized=false;
		}
	}
	else if(pauseGameToolBarButton==btn)
	{
		if(pauseGameToolBarButton->isButtonPressed() && !playGameToolBarButton->isButtonPressed())
		{
			//reset the button if not in play mode
			pauseGameToolBarButton->buttonNormal(true);
		}
	}
	else if(translateGizmoToolBarButton==btn)
	{
		if(translateGizmoToolBarButton->isButtonPressed())
		{
			rotateGizmoToolBarButton->buttonNormal(true);
			scaleGizmoToolBarButton->buttonNormal(true);
		}
	}
	else if(rotateGizmoToolBarButton==btn)
	{
		if(rotateGizmoToolBarButton->isButtonPressed())
		{
			translateGizmoToolBarButton->buttonNormal(true);
			scaleGizmoToolBarButton->buttonNormal(true);
		}
	}
	else if(scaleGizmoToolBarButton==btn)
	{
		if(scaleGizmoToolBarButton->isButtonPressed())
		{
			rotateGizmoToolBarButton->buttonNormal(true);
			translateGizmoToolBarButton->buttonNormal(true);
		}
	}
}

void gearSceneWorldEditor::onSliderChange(geGUIBase* slider)
{
	if(slider==ambientLightHorizontalSlider)
	{
		float val=0.1f+ambientLightHorizontalSlider->getSliderValue()*0.13f;
		glClearColor(val, val, val, 1.0f);
	}
	else if(slider==timeScaleHorizontalSlider)
	{
		Timer::setTimeScale(timeScaleHorizontalSlider->getSliderValue());
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
#ifdef _WIN32
	switch(cmd)
	{
	case ID_POSTPROCESSOR_BLURPROCESSOR:
		{
			isEnablePostProcessorBlur=!isEnablePostProcessorBlur;
			//EditorGEARApp::getScenePropertyEditor()->populatePropertyOfBlurShader(engine_getHWShaderManager()->GetHWShader(HW_BUILTIN_DEFAULT_SHADER_ONLY_BLUR_SHADER));
		}
		break;
	}
#endif
}

void gearSceneWorldEditor::stopSimulation()
{
	playGameToolBarButton->buttonNormal(true);
	pauseGameToolBarButton->buttonNormal(true);
	isMonoGameInitialized=false;
}

void gearSceneWorldEditor::selectedObject3D(object3d* obj)
{
	selectedObject=obj;	
	noOfSelectedObjectTriangles=0;
	getTringleCountForThisTree(obj, noOfSelectedObjectTriangles);
}

void gearSceneWorldEditor::getTringleCountForThisTree(object3d* obj, int& count)
{
	if(obj==NULL) return;

	if(obj->getID()==OBJECT3D_MESH || obj->getID()==OBJECT3D_SKINNED_MESH)
	{
		gxMesh* mesh = (gxMesh*)obj;
		count+=mesh->getTriangleCount();
	}

	std::vector<object3d*>* childList = obj->getChildList();
	for(std::vector<object3d*>::iterator it = childList->begin(); it != childList->end(); ++it)
	{
		object3d* child = *it;
		getTringleCountForThisTree(child, count);
	}
}