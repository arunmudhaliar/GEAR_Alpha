#include "gearSceneWorldEditor.h"
#include "../EditorApp.h"
#include "../core/Timer.h"

gearSceneWorldEditor::gearSceneWorldEditor():
geWindow("World Editor")
{
	m_pSelectedObj=NULL;
	m_pMainWorldPtr=NULL;

	m_pHorizontalSlider_LightAmbient=NULL;
}

gearSceneWorldEditor::~gearSceneWorldEditor()
{
	//GE_DELETE(m_pHorizontalSlider_LightAmbient);
}

void gearSceneWorldEditor::onCreate()
{
	monoWrapper::mono_engine_init(2);
	m_pMainWorldPtr = monoWrapper::mono_engine_getWorld(0);

	char metaDataFolder[512];
	memset(metaDataFolder, 0, sizeof(metaDataFolder));
	sprintf(metaDataFolder, "%s/%s", EditorApp::getProjectHomeDirectory(), "MetaData");
	monoWrapper::mono_engine_setMetaFolder(monoWrapper::mono_engine_getWorld(0), metaDataFolder);
	monoWrapper::mono_engine_setMetaFolder(monoWrapper::mono_engine_getWorld(1), metaDataFolder);

	geToolBarButton* tbtn0=new geToolBarButton("hello", getToolBar());
	tbtn0->loadImage("res//icons16x16.png", 7, 153);
	getToolBar()->appendToolBarControl(tbtn0);
	geToolBarButton* tbtn1=new geToolBarButton("hello world", getToolBar());
	tbtn1->loadImage("res//icons16x16.png", 27, 153);
	getToolBar()->appendToolBarControl(tbtn1);
	geToolBarButton* tbtn2=new geToolBarButton("1234", getToolBar());
	tbtn2->loadImage("res//icons16x16.png", 49, 153);
	getToolBar()->appendToolBarControl(tbtn2);

	m_pHorizontalSlider_LightAmbient = new geHorizontalSlider();
	m_pHorizontalSlider_LightAmbient->create(getToolBar(), "slider", 0, GE_TOOLBAR_HEIGHT*0.35f, 70);
	m_pHorizontalSlider_LightAmbient->setSliderValue(0.03f);
	getToolBar()->appendToolBarControl(m_pHorizontalSlider_LightAmbient);
}

void gearSceneWorldEditor::draw()
{
	drawTitleAndToolBar();

	monoWrapper::mono_engine_resize(m_pMainWorldPtr, m_cPos.x+getIamOnLayout()->getPos().x, (rendererGL10::g_pRendererGL10->getViewPortSz().y)-(m_cPos.y+getIamOnLayout()->getPos().y+m_cSize.y), m_cSize.x/*+2.0f*/, m_cSize.y-getTopMarginOffsetHeight()/**//*+2.0f*/);

	//glViewport(m_cPos.x+getIamOnLayout()->getPos().x, (rendererGL10::g_pRendererGL10->getViewPortSz().y)-(m_cPos.y+getIamOnLayout()->getPos().y+m_cSize.y), m_cSize.x/*+2.0f*/, m_cSize.y-getTopMarginOffsetHeight()/**//*+2.0f*/);	
	//glMatrixMode(GL_PROJECTION);
	//glPushMatrix();
	//glLoadIdentity();
	////gluOrtho2D((int)0, (int)(m_cSize.x/*+2.0f*/), (int)(m_cSize.y-getTopMarginOffsetHeight()/*+2.0f*/), (int)0);
	//gluPerspective(45, m_cSize.x/m_cSize.y, 1.0f, 1000.0f);
	//glMatrixMode(GL_MODELVIEW);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	GLfloat lightpos[] = {-1, 1, 1, 0.0f};
	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
	
	float af=m_pHorizontalSlider_LightAmbient->getSliderValue();//*0.5f;

	float colorWhite[]  = {1.0f,1.0f,1.0f,1.0f};
	float ambient[]   = {af,af,af,1.0f};//{af,af,af,1.0f};
	float diffuse[]   = {1.0f, 1.0f, 1.0f, 1.0f};
	//float diffuse[]   = {0.38f, 0.38f, 0.6f,1.0f};
	//float diffuse[]   = {0.6f, 0.6f, 0.9f,1.0f};

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient );
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse );


	glPushMatrix();
	onDraw();
	glPopMatrix();


	//STATS
	glViewport(m_cPos.x+getIamOnLayout()->getPos().x, (rendererGL10::g_pRendererGL10->getViewPortSz().y)-(m_cPos.y+getIamOnLayout()->getPos().y+m_cSize.y), m_cSize.x, m_cSize.y-getTopMarginOffsetHeight());	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D((int)0, (int)(m_cSize.x), (int)(m_cSize.y-getTopMarginOffsetHeight()), (int)0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushMatrix();
	glTranslatef(0, 0, -1);
	char buffer[16];
	sprintf(buffer, "FPS : %3.2f", Timer::getFPS());
	glDisable(GL_DEPTH_TEST);
	geGUIManager::g_pFontArial12Ptr->drawString(buffer, 0, 0+geGUIManager::g_pFontArial12Ptr->getLineHeight(), m_cSize.x);

	//m_pHorizontalSlider_LightAmbient->draw();

	glEnable(GL_DEPTH_TEST);
	glPopMatrix();
	//
}

void drawGizmo(float scale);
void gearSceneWorldEditor::onDraw()
{
	monoWrapper::mono_engine_update(m_pMainWorldPtr, 1.0f);
	monoWrapper::mono_engine_render(m_pMainWorldPtr);

	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);

	//glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE); 
	//glEnable(GL_COLOR_MATERIAL);

	if(m_pSelectedObj)
	{
		glPushMatrix();
		glMultMatrixf(m_pSelectedObj->getWorldMatrix()->getMatrix());
			drawGizmo(3.0f);
			glColor4f(0.25f, 0.4f, 0.62f, 1);
			m_pSelectedObj->getAABB().draw();
		glPopMatrix();
	}


	drawGizmo(3.0f);
	//glDisable(GL_COLOR_MATERIAL);
}

void gearSceneWorldEditor::onSize(float cx, float cy, int flag)
{
	//if(m_pHorizontalSlider_LightAmbient)
	//	m_pHorizontalSlider_LightAmbient->setPos(cx-100, 30);
	geWindow::onSize(cx, cy, flag);
}

bool gearSceneWorldEditor::onMouseLButtonDown(float x, float y, int nFlag)
{
	monoWrapper::mono_engine_mouseLButtonDown(m_pMainWorldPtr, x, y, nFlag);
	return true;
}

bool gearSceneWorldEditor::onMouseLButtonUp(float x, float y, int nFlag)
{
	monoWrapper::mono_engine_mouseLButtonUp(m_pMainWorldPtr, x, y, nFlag);
	return true;
}

bool gearSceneWorldEditor::onMouseRButtonDown(float x, float y, int nFlag)
{
	monoWrapper::mono_engine_mouseRButtonDown(m_pMainWorldPtr, x, y, nFlag);
	return true;
}

void gearSceneWorldEditor::onMouseRButtonUp(float x, float y, int nFlag)
{
	monoWrapper::mono_engine_mouseRButtonUp(m_pMainWorldPtr, x, y, nFlag);
}

void gearSceneWorldEditor::onMouseMove(float x, float y, int flag)
{
	monoWrapper::mono_engine_mouseMove(m_pMainWorldPtr, x, y, flag);
}

void gearSceneWorldEditor::onMouseWheel(int zDelta, int x, int y, int flag)
{
	monoWrapper::mono_engine_mouseWheel(m_pMainWorldPtr, zDelta, x, y, flag);
}

void drawGizmo(float scale)
{
	float lines[]=
	{
		0.0f, 0.0f, 0.0f,
		scale, 0.0f, 0.0f,

		0.0f, 0.0f, 0.0f,
		0.0f, scale, 0.0f,

		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, scale
	};

	float color[]=
	{
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f
	};

	glDisable(GL_DEPTH_TEST);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, lines);

	glEnableClientState(GL_COLOR_ARRAY);
	glColorPointer(3, GL_FLOAT, 0, color);

	glDrawArrays(GL_LINES, 0, 6);

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glEnable(GL_DEPTH_TEST);
}