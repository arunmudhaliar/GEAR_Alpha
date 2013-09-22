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
}

void gearScenePreview::draw()
{
	drawTitleAndToolBar();

	if(!m_pSelectedObj) return;

	monoWrapper::mono_engine_resize(m_pPreviewWorldPtr, m_cPos.x+getIamOnLayout()->getPos().x, (rendererGL10::g_pRendererGL10->getViewPortSz().y)-(m_cPos.y+getIamOnLayout()->getPos().y+m_cSize.y), m_cSize.x/*+2.0f*/, m_cSize.y-getTopMarginOffsetHeight()/**//*+2.0f*/);

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

	glPushMatrix();
	onDraw();
	glPopMatrix();

	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);


	//STATS
	glViewport(m_cPos.x+getIamOnLayout()->getPos().x, (rendererGL10::g_pRendererGL10->getViewPortSz().y)-(m_cPos.y+getIamOnLayout()->getPos().y+m_cSize.y), m_cSize.x, m_cSize.y-getTopMarginOffsetHeight());	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D((int)0, (int)(m_cSize.x), (int)(m_cSize.y-getTopMarginOffsetHeight()), (int)0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushMatrix();
	glTranslatef(0, 0, -1);

	//gxMesh* mesh = (gxMesh*)m_pSelectedObj;

	//char buffer[16];
	//sprintf(buffer, "Tris: %d", mesh->getTotalNoOfTris());
	glDisable(GL_DEPTH_TEST);
	geGUIManager::g_pFontArial12Ptr->drawString("1 object selected", 5, 5+geGUIManager::g_pFontArial12Ptr->getLineHeight(), m_cSize.x);
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

	monoWrapper::mono_engine_renderSingleObject(m_pPreviewWorldPtr, m_pSelectedObj);
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