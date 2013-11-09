#include "gearScenePreview.h"
#include "../EditorApp.h"
#include "../core/Timer.h"

gearScenePreview::gearScenePreview():
geWindow("Preview")
{
	m_pSelectedObj=NULL;
	m_pPreviewWorldPtr=NULL;
}

gearScenePreview::~gearScenePreview()
{
}

void gearScenePreview::onCreate()
{
	m_pPreviewWorldPtr=monoWrapper::mono_engine_getWorld(1);
	object3d* light=engine_createLight(m_pPreviewWorldPtr, "Point Light", gxLight::LIGHT_POINT);
	((gxLight*)light)->setDiffuseColor(vector4f(0.5f, 0.5f, 0.5f, 1.0f));
	((gxLight*)light)->setAmbientColor(vector4f(0.2f, 0.2f, 0.2f, 1.0f));
}

void gearScenePreview::draw()
{
	drawTitleAndToolBar();

	if(!m_pSelectedObj) return;

	monoWrapper::mono_engine_resize(m_pPreviewWorldPtr, m_cPos.x+getIamOnLayout()->getPos().x, (m_pRenderer->getViewPortSz().y)-(m_cPos.y+getIamOnLayout()->getPos().y+m_cSize.y), m_cSize.x/*+2.0f*/, m_cSize.y-getTopMarginOffsetHeight()/**//*+2.0f*/);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	GLfloat lightpos[] = {-1, 1, 1, 0.0f};
	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
	
	float colorWhite[]  = {1.0f,1.0f,1.0f,1.0f};
	float ambient[]   = {0.5f,0.5f,0.5f,1.0f};
	float diffuse[]   = {1.0f,1.0f,1.0f,1.0f};
	float specular[]   = {0.1f,0.1f,0.1f,1.0f};
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient );
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse );
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular );

	glEnable(GL_COLOR_MATERIAL);
	glColor4f(0.7f, 0.7f, 0.7f, 1.0f);
	glPushMatrix();
	onDraw();
	glPopMatrix();
	glDisable(GL_COLOR_MATERIAL);

	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);


	//STATS
	glViewport(m_cPos.x+getIamOnLayout()->getPos().x, (m_pRenderer->getViewPortSz().y)-(m_cPos.y+getIamOnLayout()->getPos().y+m_cSize.y), m_cSize.x, m_cSize.y-getTopMarginOffsetHeight());	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D((int)0, (int)(m_cSize.x), (int)(m_cSize.y-getTopMarginOffsetHeight()), (int)0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushMatrix();
	glTranslatef(0, 0, -1);

	//gxMesh* mesh = (gxMesh*)m_pSelectedObj;

	//char buffer[16];
	//sprintf(buffer, "Tris: %d", mesh->getVerticesCount()/3);
	glDisable(GL_DEPTH_TEST);
	geGUIManager::g_pFontArial10_84Ptr->drawString("1 object selected", 5, 5+geGUIManager::g_pFontArial10_84Ptr->getLineHeight(), m_cSize.x);
	glEnable(GL_DEPTH_TEST);
	//GLUquadric* q=gluNewQuadric();
	//gluCylinder(q, 10, 0, 20, 20, 5);
	//gluDeleteQuadric(q);
	glPopMatrix();
	//
}

void gearScenePreview::onDraw()
{
	monoWrapper::mono_engine_update(m_pPreviewWorldPtr, 1.0f);

	m_pPreviewWorldPtr->getRenderer()->setRenderPassType(gxRenderer::RENDER_NORMAL);
	monoWrapper::mono_engine_renderSingleObject(m_pPreviewWorldPtr, m_pSelectedObj, NULL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	HWShaderManager* hwManager = engine_getHWShaderManager();
	gxHWShader* shader=hwManager->GetHWShader(3);
	shader->enableProgram();
	std::vector<gxLight*>* lightList = m_pPreviewWorldPtr->getLightList();
	for(int x=0;x<(int)lightList->size();x++)
	{
		gxLight* light = lightList->at(x);
		if(!light->isBaseFlag(object3d::eObject3dBaseFlag_Visible))
			continue;
		light->renderPass(m_pPreviewWorldPtr->getRenderer(), shader);

		m_pPreviewWorldPtr->getRenderer()->setRenderPassType(gxRenderer::RENDER_LIGHTING_ONLY);
		//Note:- glDepthFunc(GL_LEQUAL); by default its GL_LEQUAL in engine so no need to change here
		monoWrapper::mono_engine_renderSingleObject(m_pPreviewWorldPtr, m_pSelectedObj, light);
	}
	shader->disableProgram();
	glDisable(GL_BLEND);
}

bool gearScenePreview::onMouseLButtonDown(float x, float y, int nFlag)
{
	monoWrapper::mono_engine_mouseLButtonDown(m_pPreviewWorldPtr, x, y, nFlag);
	return true;
}

bool gearScenePreview::onMouseLButtonUp(float x, float y, int nFlag)
{
	monoWrapper::mono_engine_mouseLButtonUp(m_pPreviewWorldPtr, x, y, nFlag);
	return true;
}

bool gearScenePreview::onMouseRButtonDown(float x, float y, int nFlag)
{
	monoWrapper::mono_engine_mouseRButtonDown(m_pPreviewWorldPtr, x, y, nFlag);
	return true;
}

void gearScenePreview::onMouseRButtonUp(float x, float y, int nFlag)
{
	monoWrapper::mono_engine_mouseRButtonUp(m_pPreviewWorldPtr, x, y, nFlag);
}

bool gearScenePreview::onMouseMove(float x, float y, int flag)
{
	monoWrapper::mono_engine_mouseMove(m_pPreviewWorldPtr, x, y, flag);

	return true;
}

void gearScenePreview::onMouseWheel(int zDelta, int x, int y, int flag)
{
	monoWrapper::mono_engine_mouseWheel(m_pPreviewWorldPtr, zDelta, x, y, flag);
}