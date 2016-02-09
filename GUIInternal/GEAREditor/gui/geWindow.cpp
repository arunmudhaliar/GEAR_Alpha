#include "geWindow.h"
#include "geGUIManager.h"

geWindow::geWindow(const char* name, geFontManager* fontmanager):
	geGUIBase(GEGUI_WINDOW, name, fontmanager)
{
	is_Movable=false;
	toolBar=NULL;
}

geWindow::~geWindow()
{
	GE_DELETE(toolBar);
}

void geWindow::create(rendererGL10* renderer, geGUIBase* parent, float x, float y, float cx, float cy, bool bCreateToolBar)
{
	createBase(renderer, parent);

	titleWidth=0;
	for(int index=0;index<(int)strlen(m_szName);index++)
	{
		titleWidth+=geFontManager::g_pFontArial10_84Ptr->getCharWidth(m_szName[index]);
	}

	is_Movable=false;
	iamOnLayout=NULL;
	setPos(x, y);
	setSize(cx, cy);
	setTitleColor(0.12f, 0.12f, 0.12f, 1.0f);
	setClientColor(0.2f, 0.2f, 0.2f, 1.0f);
	setSizable(true);
	toolBar=NULL;
	if(bCreateToolBar)
	{
		toolBar = new geToolBar(fontManagerGUI);
		toolBar->create(renderer, this, 0, -GE_TOOLBAR_HEIGHT/*GE_WND_TITLE_HEIGHT*/, cx, GE_TOOLBAR_HEIGHT);
	}

	roundedRectControl.create(titleWidth+30, GE_WND_TITLE_HEIGHT-3, 5);

	onCreate();
}

void geWindow::show()
{
}

void geWindow::hide()
{
}

void geWindow::enable()
{
}

void geWindow::disable()
{
}

void geWindow::update(float dt)
{
}

void geWindow::setTitleColor(float r, float g, float b, float a)
{
	setColor(&vertexBufferTitle, r, g, b, a);
}

void geWindow::setClientColor(float r, float g, float b, float a)
{
	setClientAreaPrimaryActiveForeColor(r, g, b, a);
	applyPrimaryColorToVBClientArea();
}

void geWindow::drawTitleAndToolBar(float xoff, float yoff, bool bActiveWindow, bool bFirstWindow)
{
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(m_cPos.x+xoff, m_cPos.y+yoff, 0);
	if(bFirstWindow)
	{
		drawRect(&vertexBufferTitle);
	}

	if(bActiveWindow)
	{
		roundedRectControl.draw(1, 3);
	}
	geFontManager::g_pFontArial10_84Ptr->drawString(m_szName, 15, geFontManager::g_pFontArial10_84Ptr->getLineHeight(), m_cSize.x);
	//drawTriangle(&vertexBufferToggleButtonArray[3*0], 0.05f, 0.05f, 0.05f, 1.0f, 3);
	if(toolBar && bActiveWindow)
	{
		glTranslatef(-xoff, -yoff, 0);
		toolBar->draw();
	}
	glPopMatrix();
}

void geWindow::draw()
{
	glViewport(m_cPos.x+iamOnLayout->getPos().x, (rendererGUI->getViewPortSz().y)-(m_cPos.y+iamOnLayout->getPos().y+m_cSize.y), m_cSize.x/*+2.0f*/, m_cSize.y-getTopMarginOffsetHeight()/**//*+2.0f*/);	
	glMatrixMode(GL_PROJECTION);
	//glPushMatrix();
		glLoadIdentity();
		gluOrtho2D((int)0, (int)(m_cSize.x/*+2.0f*/), (int)(m_cSize.y-getTopMarginOffsetHeight()/*+2.0f*/), (int)0);
		glMatrixMode(GL_MODELVIEW);

		drawRect(&vertexBufferClientArea);
		onDraw();
	//glPopMatrix();
}



void geWindow::clearVarsAfterWindowMoved()
{
	is_Movable = false;
}

//bool geWindow::selectWindow(int x, int y)
//{
//	bool bSelected = isPointInsideWindow(x, y);
//	is_Movable = isPointInsideWindowTitle(x, y);
//
//	return bSelected;
//}

bool geWindow::isPointInsideWindowTitle(int x, int y)
{
	return (x>m_cPos.x && x<m_cPos.x+m_cSize.x && y>m_cPos.y && y<m_cPos.y+GE_WND_TITLE_HEIGHT);
}

void geWindow::onCreate()
{
}

void geWindow::onPosition(float x, float y, int flag)
{
}

void geWindow::onSize(float cx, float cy, int flag)
{
	const float title_vertLst[8] =
	{
		cx,	0,
		0,	0,
		cx,	GE_WND_TITLE_HEIGHT,
		0,	GE_WND_TITLE_HEIGHT,
	};
	memcpy(vertexBufferTitle.vertexArray, title_vertLst, sizeof(title_vertLst));

	float cy_val=cy-getTopMarginOffsetHeight();
	if(cy_val<=0.0f)cy_val=0.0f;
	const float clientarea_vertLst[8] =
	{
		cx,	0,
		0,	0,
		cx,	cy_val,
		0,	cy_val,
	};
	memcpy(vertexBufferClientArea.vertexArray, clientarea_vertLst, sizeof(clientarea_vertLst));


	const float clientarea_linevertLst[8] =
	{
		cx-1,	1/*GE_WND_TITLE_HEIGHT*/,
		1,		1/*GE_WND_TITLE_HEIGHT*/,
		1,		cy_val-1,
		cx-1,	cy_val-1,
	};
	memcpy(vertexBufferClientAreaArray, clientarea_linevertLst, sizeof(clientarea_linevertLst));

	int h=GE_WND_TITLE_HEIGHT-4;
	const float togglebutton_linevertLst[6*2] =
	{
		5,		(GE_WND_TITLE_HEIGHT-4),
		5+8,	(GE_WND_TITLE_HEIGHT-4)-(h>>2),
		5,		(GE_WND_TITLE_HEIGHT-4)-(h>>1),

		5+4,	(GE_WND_TITLE_HEIGHT-4),
		5+8,	(GE_WND_TITLE_HEIGHT-4)-(h>>1),
		5,		(GE_WND_TITLE_HEIGHT-4)-(h>>1),
	};
	memcpy(vertexBufferToggleButtonArray, togglebutton_linevertLst, sizeof(togglebutton_linevertLst));

	if(toolBar)
		toolBar->setSize(cx, GE_TOOLBAR_HEIGHT);

	geGUIBase::onSize(cx, cy, flag);
}

void geWindow::onDraw()
{
}

void geWindow::onDestroy()
{
}

bool geWindow::onMouseLButtonDown(float x, float y, int nFlag)
{
	return geGUIBase::onMouseLButtonDown(x, y, nFlag);
}

bool geWindow::onMouseLButtonUp(float x, float y, int nFlag)
{
	return geGUIBase::onMouseLButtonUp(x, y, nFlag);
}

bool geWindow::onMouseMove(float x, float y, int flag)
{
	return geGUIBase::onMouseMove(x, y, flag);
}

void geWindow::onMouseEnterClientArea()
{
}

void geWindow::onMouseExitClientArea()
{
	//CancelEngagedControls();
}

geVector2f geWindow::getAbsolutePositionOnScreen()
{
	return geVector2f(m_cPos.x+iamOnLayout->getPos().x, m_cPos.y+iamOnLayout->getPos().y);
}
